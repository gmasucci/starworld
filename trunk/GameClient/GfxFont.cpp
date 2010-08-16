#include "stdafx.h"
#include "GfxFont.h"
#include <stdio.h>


__inline float _floor(float f)
{
	static int _n;
	_asm fld f
	_asm fistp _n
	return (float)_n;
}

// 65���Ҷȱ�
const unsigned char g_byAlphaLevel[65] = 
{
	  0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
	 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
	104,108,112,116,120,124,128,132,136,140,144,148,152,
	156,160,164,168,172,176,180,184,188,192,196,200,204,
	208,212,216,220,224,228,232,236,240,244,248,252,255
};

GfxFont::GfxFont( _lpctstr lpsFontName, int nFaceSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);

	// ����GDI����豸
	HDC hDC = GetDC(m_pHGE->System_GetState(HGE_HWND));
	m_hMemDC = CreateCompatibleDC(hDC);
	if (NULL == m_hMemDC) return;
	ReleaseDC(m_pHGE->System_GetState(HGE_HWND),hDC);

	::SetMapMode(m_hMemDC, MM_TEXT);
	::SetTextColor(m_hMemDC,RGB(255,255,255));
	::SetBkColor(m_hMemDC,RGB(0,0,0));

	m_hFont = CreateFont(
		-nFaceSize,
		0,
		0,
		0,
		(bBold) ? FW_BOLD : FW_NORMAL,
		bItalic,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		lpsFontName);
	if (NULL == (m_hFont)) return;
	SelectObject(m_hMemDC, m_hFont);

	memset(m_Glyphs,0,sizeof(TENGINEFONTGLYPH)*font_count);

	m_nAntialias	= bAntialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;

	TEXTMETRIC tm;
	::GetTextMetrics(m_hMemDC,&tm);
	m_nAscent		= tm.tmAscent;

	m_nFontSize		= nFaceSize;
	m_nKerningWidth	= 0;
	m_nKerningHeight= 0;

	m_pSprite = new hgeSprite(0, 0, 0, 0, 0);
	m_pSprite->SetColor(ARGB(255, 255, 255, 255));
}

GfxFont::~GfxFont(void)
{
	for (int nIdx = 0; nIdx < font_count; ++nIdx)
	{ if (m_Glyphs[nIdx].t) m_pHGE->Texture_Free(m_Glyphs[nIdx].t); }

	if ((m_hFont)) DeleteObject(m_hFont);
	if ((m_hMemDC)) DeleteDC(m_hMemDC);

	if(m_pSprite) delete m_pSprite;
	if(m_pHGE) m_pHGE->Release();
}

// ��Ⱦ�ı�
void GfxFont::Print( float x, float y, const char *format, ... )
{
	char sBuffer[10240] = {0};
	char *lpsArg=(char*)&format+sizeof(format);
	_vsnprintf( sBuffer, _countof(sBuffer), format, lpsArg);

	USES_CONVERSION;
	Render(x,y,A2W(sBuffer));
}

void GfxFont::Render(float x, float y, const wchar_t* text )
{
	float offsetX = x;
	float offsetY = y;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
		}
		else
		{
			Render( offsetX, offsetY, *text );
			offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
		}

		++text;
	}
}

inline void GfxFont::Render(float x, float y, const wchar_t text )
{
	unsigned int idx = GetGlyphByCharacter(text);
	if (idx > 0)
	{
		m_pSprite->SetTexture(m_Glyphs[idx].t);
		m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
		m_pSprite->Render(x - m_Glyphs[idx].x, y - m_Glyphs[idx].y);
	}
}

// �������ȡ��ɫ
void GfxFont::SetColor( _uint32 dwColor, int i )
{
	m_pSprite->SetColor(dwColor,i);
}

_uint32 GfxFont::GetColor(int i)
{
	return m_pSprite->GetColor(i);
}

// ��ȡ�ı����
iSize GfxFont::GetTextSize( const wchar_t* text )
{
	iSize dim( 0, m_nFontSize );
	int nRowWidth = 0;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			dim.cy += m_nFontSize + m_nKerningHeight;
			if (dim.cx < nRowWidth)
				dim.cx = nRowWidth;
			nRowWidth = 0;
		}
		else
			nRowWidth += (GetWidthFromCharacter(*text) + m_nKerningWidth);
		++text;
	}

	if (dim.cx < nRowWidth)
		dim.cx = nRowWidth;

	return dim;
}

// ��ȡ�ַ����
int GfxFont::GetCharacterWidth( const wchar_t character )
{
	return GetWidthFromCharacter(character) + m_nKerningWidth;
}

// ��ȡ�ַ��߶�
int GfxFont::GetCharacterHeight()const
{
	return m_nFontSize + m_nKerningHeight;
}

// ���������ȡ�ַ�
wchar_t GfxFont::GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y )
{
	int x = 0;
	int y = 0;

	while (*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			x = 0;
			y += (m_nFontSize+m_nKerningHeight);
			text++;

			if (!(*text))
				break;
		}

		int w = GetWidthFromCharacter(*text);
		if (pixel_x > x && pixel_x <= x + w &&
			pixel_y > y && pixel_y <= y + m_nFontSize)
			return *text;

		x += (w+m_nKerningWidth);

		text++;
	}

	return L'\0';
}

// �����ּ��
void GfxFont::SetKerningWidth( int kerning )
{
	m_nKerningWidth = kerning;
}
void GfxFont::SetKerningHeight( int kerning )
{
	m_nKerningHeight = kerning;
}

// ��ȡ�ּ��
int GfxFont::GetKerningWidth()const
{
	return m_nKerningWidth;
}

int GfxFont::GetKerningHeight()const
{
	return m_nKerningHeight;
}	

// �����С
int GfxFont::GetFontSize()const
{
	return m_nFontSize;
}

// �����ַ���ȡ����
unsigned int GfxFont::GetGlyphByCharacter( wchar_t c )
{
	unsigned int idx = (unsigned int)c;
	if (NULL == (m_Glyphs[idx].t)) CacheCharacter(idx,c);
	return idx;
}

inline int GfxFont::GetWidthFromCharacter( wchar_t c, bool original )
{
	unsigned int idx = GetGlyphByCharacter(c);
	if (original && idx > 0 && idx < font_count) return m_Glyphs[idx].c;
	return	(idx >= 0x2000) ? m_nFontSize : (int)_floor(m_nFontSize / 2.0f);
}

inline void GfxFont::CacheCharacter(unsigned int idx, wchar_t c)
{
	if (idx < font_count && NULL == m_Glyphs[idx].t)
	{
		UINT nChar = (UINT)c;

		MAT2 mat2 = {{0,1},{0,0},{0,0},{0,1}};
		GLYPHMETRICS gm;
		_uint32 nLen = ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,0,NULL,&mat2);

		HTEXTURE hTex = m_pHGE->Texture_Create(gm.gmBlackBoxX,gm.gmBlackBoxY);
		if (NULL == hTex) return;

		if((signed)nLen > 0)
		{
			LPBYTE lpBuf = new BYTE[nLen];

			if (nLen == ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,nLen,lpBuf,&mat2))
			{
				BYTE*	lpSrc = lpBuf;
				DWORD*	lpDst = m_pHGE->Texture_Lock(hTex,FALSE);

				if (GGO_BITMAP == m_nAntialias)
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 32 + (gm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							for(UINT k = 0; k < 8; ++k)   
							{
								UINT i = 8 * x + k;
								if (i >= gm.gmBlackBoxX)
								{
									x+=7;
									break;
								}
								lpDst[i] = ((lpSrc[x] >> (7 - k)) & 1) ? 0xFFFFFFFF : 0x0;
							}
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}
				else
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 4 + (gm.gmBlackBoxX % 4 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							lpDst[x] = ARGB(g_byAlphaLevel[lpSrc[x]],0xFF,0xFF,0xFF);
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}

				m_pHGE->Texture_Unlock(hTex);
			}

			delete lpBuf;
		}
		else
		{
			// ��������ʾ�ַ�
		}

		m_Glyphs[idx].t = hTex;
		m_Glyphs[idx].w = (float)gm.gmBlackBoxX;
		m_Glyphs[idx].h = (float)gm.gmBlackBoxY;
		m_Glyphs[idx].x = (float)-gm.gmptGlyphOrigin.x;
		m_Glyphs[idx].y = (float)(-m_nAscent + gm.gmptGlyphOrigin.y);
		m_Glyphs[idx].c = gm.gmCellIncX;
	}
}

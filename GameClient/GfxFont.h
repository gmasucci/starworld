#ifndef GDIFONT_H
#define GDIFONT_H

class GfxFont
{
public:
	GfxFont( _lpctstr lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~GfxFont(void);

public:
	// ��Ⱦ�ı�
	void	Print( float x, float y, const char *format, ... );
	void	Render(float x, float y, const wchar_t* text );
	inline void	Render(float x, float y, const wchar_t text );

	// �������ȡ��ɫ
	void	SetColor( _uint32 dwColor, int i = -1 );
	_uint32	GetColor( int i = 0 );

	// ��ȡ�ı����
	SIZE	GetTextSize( const wchar_t* text );

	LONG	GetCharacterWidth( const wchar_t character );
	LONG	GetCharacterHeight()const;

	// ���������ȡ�ַ�
	wchar_t	GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y );

	// �����ּ��
	void	SetKerningWidth( float kerning );
	void	SetKerningHeight( float kerning );

	// ��ȡ�ּ��
	float	GetKerningWidth()const;
	float	GetKerningHeight()const;

	// �����С
	float	GetFontSize()const;

private:
	// �����ַ���ȡ����
	unsigned int	GetGlyphByCharacter( wchar_t c );
	inline float	GetWidthFromCharacter( wchar_t c, bool original = false );
	inline void		CacheCharacter(unsigned int idx, wchar_t c);

	typedef struct tagEngineFontGlyph
	{
		HTEXTURE	t;
		float		w;
		float		h;
		float		x;
		float		y;
		float		c;
	}TENGINEFONTGLYPH;

	static const unsigned int font_count = 0xFFFF;// = sizeof(wchar_t);
	TENGINEFONTGLYPH	m_Glyphs[font_count];
	UINT				m_nAntialias;//�����
	LONG				m_nAscent;//����
	_uint32				m_dwFontColor;
	float				m_nFontSize;
	float				m_nKerningWidth;
	float				m_nKerningHeight;

	HGE*				m_pHGE;
	hgeSprite*			m_pSprite;

	// GDI�豸
	HDC					m_hMemDC;
	HFONT				m_hFont;
};
#endif//GDIFONT_H

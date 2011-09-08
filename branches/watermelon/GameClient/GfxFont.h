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
	iSize	GetTextSize( const wchar_t* text );

	int GetCharacterWidth( const wchar_t character );
	int GetCharacterHeight()const;

	// ���������ȡ�ַ�
	wchar_t	GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y );

	// �����ּ��
	void	SetKerningWidth( int kerning );
	void	SetKerningHeight( int kerning );

	// ��ȡ�ּ��
	int	GetKerningWidth()const;
	int	GetKerningHeight()const;

	// �����С
	int	GetFontSize()const;

private:
	// �����ַ���ȡ����
	unsigned int	GetGlyphByCharacter( wchar_t c );
	inline int		GetWidthFromCharacter( wchar_t c, bool original = false );
	inline void		CacheCharacter(unsigned int idx, wchar_t c);

	typedef struct tagEngineFontGlyph
	{
		HTEXTURE	t;
		float		w;
		float		h;
		float		x;
		float		y;
		short		c;
	}TENGINEFONTGLYPH;

	static const unsigned int font_count = 0xFFFF;// = sizeof(wchar_t);
	TENGINEFONTGLYPH	m_Glyphs[font_count];
	int					m_nAntialias;//�����
	int					m_nAscent;//����
	_uint32				m_dwFontColor;
	int					m_nFontSize;
	int					m_nKerningWidth;
	int					m_nKerningHeight;

	HGE*				m_pHGE;
	hgeSprite*			m_pSprite;

	// GDI�豸
	HDC					m_hMemDC;
	HFONT				m_hFont;
};
#endif//GDIFONT_H

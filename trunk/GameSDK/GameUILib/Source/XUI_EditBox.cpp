#include "GuiHeader.h"
#include "GuiSystem.h"
#include "XUI_EditBox.h"

namespace UILib
{
	//BEGIN_PROPERTY_MAP( CXEditBox, XUI_Wnd );
	//	PROPERTY_BIND( _T("Text"),		m_strText );
	//	PROPERTY_BIND_METHOD( _T("FontName"),	m_strFontName,	OnFontNameChange	);
	//	PROPERTY_BIND_METHOD( _T("FontSize"),	m_dwFontSize,	OnFontSizeChange	);
	//	PROPERTY_BIND_METHOD( _T("Bold"),		m_bBold,		OnFontBoldChange	);
	//	PROPERTY_BIND_METHOD( _T("Italic"),		m_bItalic,		OnFontItalicChange	);
	//END_PROPERTY_MAP()

	BEGIN_UIMSG_MAP( CXEditBox, XUI_Wnd )
	END_UIMSG_MAP()

	CXEditBox::CXEditBox(void)
	: m_bControl( false )
	, m_bShift( false )
	{
		m_FirstVisiblePos	= 
		m_CaratPos			= 0;
	}

	CXEditBox::~CXEditBox(void)
	{
	}

	//�ػ棬ͨ��ʵ��������������ֿռ�����
	void CXEditBox::RenderSelf(const CRect& clipper)
	{
		int nX = m_wndRect.left;
		int nY = m_wndRect.top;

		XUI_DrawRect( m_wndRect, 0xff00ff00, 0xff00ff00 );
		CRect rc;
		rc.IntersectRect( m_wndRect, clipper );

		_string::size_type end, begin = m_FirstVisiblePos;
		while( ( end = m_strText.find( _T('\r'), begin ) ) != _string::npos )
		{
			XUI_DrawText( m_strText.substr( begin, end - begin ).c_str(), m_pFont, nX, nY, 0, rc );
			begin = end+1;
		}
		XUI_DrawText( m_strText.substr( begin ).c_str(), m_pFont, nX, nY, 0, rc );
	}

	//���
	//����ƶ��¼�
	//����˵����
	//pt���������꣬����ڿؼ�
	//sysKeys��������Ҫ������״̬���μ�MSDN	
	bool CXEditBox::onMouseMove(const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	bool CXEditBox::onMouseHover(const CPoint& pt)
	{
		return true;
	}

	bool CXEditBox::onMouseEnter()
	{
		return true;
	}

	bool CXEditBox::onMouseLeave()
	{
		return true;
	}

	//��갴���¼�
	//����˵����
	//button�����µļ���0-�����1-�Ҽ���2-�м�
	//pt����������
	//sysKeys��������Ҫ������״̬���μ�MSDN
	bool CXEditBox::onButtonDown(int button, const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	bool CXEditBox::onButtonUp(int button, const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	//����
	//����˵��
	//keycode�����µļ�
	//sysKeys��������Ҫ������״̬���μ�MSDN
	bool CXEditBox::onKeyDown(DWORD keycode, UINT sysKeys)
	{
		switch( keycode )
		{
		case VK_CONTROL:
			m_bControl = true;
			break;
		case VK_SHIFT:
		case VK_RSHIFT:
			m_bShift = true;
			break;
		case VK_BACK:
			HandleBack( sysKeys );
			break;
		case VK_DELETE:
			HandleDelete( sysKeys );
			break;
		case VK_HOME:
			HandleHome( sysKeys );
			break;
		case VK_END:
			HandleEnd( sysKeys );
			break;
		case VK_LEFT:
			if( m_bControl )
				HandleWordLeft( sysKeys );
			else
				HandleCharLeft( sysKeys );
			break;
		case VK_RIGHT:
			if( m_bControl )
				HandleWordRight( sysKeys );
			else
				HandleCharRight( sysKeys );
			break;
		case VK_RETURN:
			m_strText.insert( m_CaratPos++, 1, _T('\n') );
			HandleReturn( sysKeys );
			break;
		}
		return true;
	}

	void CXEditBox::HandleBack( UINT nSysKey )
	{
		if( m_CaratPos <= 0 ) return;
		m_strText.erase( --m_CaratPos, 1 );
	}

	void CXEditBox::HandleDelete( UINT nSysKey )
	{
		if( m_CaratPos >= m_strText.length() ) return;
		m_strText.erase( m_CaratPos, 1 );
	}

	void CXEditBox::HandleHome( UINT nSysKey )
	{
		while( m_CaratPos && m_strText[m_CaratPos] != _T('\n') ) --m_CaratPos;
	}

	void CXEditBox::HandleEnd( UINT nSysKey )
	{
		while( m_CaratPos != m_strText.length() && m_strText[m_CaratPos] != _T('\n') ) ++m_CaratPos;
	}

	void CXEditBox::HandleWordLeft( UINT nSysKey )
	{
		while( m_CaratPos && m_strText[m_CaratPos] != _T(' ') ) --m_CaratPos;
	}

	void CXEditBox::HandleCharLeft( UINT nSysKey )
	{
		if( m_CaratPos > 0 ) --m_CaratPos; 
	}

	void CXEditBox::HandleWordRight( UINT nSysKey )
	{
		while( m_CaratPos != m_strText.length() && m_strText[m_CaratPos] != _T(' ') ) ++m_CaratPos;
	}

	void CXEditBox::HandleCharRight( UINT nSysKey )
	{
		if( m_CaratPos <= m_strText.length() ) ++m_CaratPos;
	}

	void CXEditBox::HandleReturn( UINT nSysKey )
	{
	}

	bool CXEditBox::onKeyUp( DWORD keycode, UINT sysKeys )
	{
		switch( keycode )
		{
		case VK_CONTROL:
			m_bControl = false;
			break;
		case VK_SHIFT:
		case VK_RSHIFT:
			m_bShift = false;
			break;
		}
		return true;
	}

	//����˵��
	//c��������ַ�
	//sysKeys��������Ҫ������״̬���μ�MSDN
	bool CXEditBox::onChar(DWORD c, UINT sysKeys)
	{
		if( _istprint( LOWORD(c) ) )
		{
			m_strText.insert( m_CaratPos++, 1, (wchar_t)c );
		}
		return true;
	}

	//���뷨
	//�μ�MSDN
	bool CXEditBox::onImeComp(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	bool CXEditBox::onImeEndComp(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	bool CXEditBox::onImeNotify(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	unsigned int CXEditBox::BiuldFont()
	{
		XUI_DestroyFont( m_pFont );
		//m_pFont = XUI_CreateFont( m_strFontName.c_str(), m_dwFontSize, m_bBold, m_bItalic, m_bAntialias );
		return 0;
	}

	unsigned int CXEditBox::OnMoveWindow( CRect& rcWindow )
	{
		return XUI_Wnd::OnMoveWindow( rcWindow );
	}
}
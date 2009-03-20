#include "GuiHeader.h"
#include "GuiSystem.h"
#include "XUI_EditBox.h"

namespace UILib
{
	BEGIN_UIMSG_MAP( XUI_EditBox, XUI_Wnd )
	END_UIMSG_MAP()

	XUI_EditBox::XUI_EditBox(void)
	: UIObjTypeT< XUI_Wnd, TypeEditBox >()
	, m_bControl( false )
	, m_bShift( false )
	, m_CaratPos( 0 )
	, m_FirstLineNumber( 0 )
	, m_nCurLineNumber( 0 )
	, m_bShowCarat( true )
	{
		XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();
		if( pFont )
		{
			m_WindowSize.cx		= m_wndRect.Width()/pFont->GetCharacterWidth( _T(' ') );
			m_WindowSize.cy		= m_wndRect.Height()/pFont->GetCharacterHeight();
		}

		m_CaratTimerHandler = GuiSystem::Instance().SetTimer( event_function( this, &XUI_EditBox::CaratTimerUpdate ), 1, TIMER_SECOND(0.5f) );
	}

	XUI_EditBox::~XUI_EditBox(void)
	{
		GuiSystem::Instance().KillTimer( m_CaratTimerHandler );
	}

	bool XUI_EditBox::CaratTimerUpdate( unsigned int handle, unsigned short& repeat, unsigned int& timer )
	{
		m_bShowCarat = !m_bShowCarat;
		repeat = 1;
		return true;
	}

	void XUI_EditBox::RenderCharacter( TCHAR szChar, XUI_IFont* pFont, LONG &x, LONG &y, BOOL bRender )
	{
		if( bRender )
		{
			XUI_DrawCharacter( szChar, pFont, (float)x, (float)y );
		}

		x += pFont->GetCharacterWidth( szChar );
	}

	//�ػ棬ͨ��ʵ��������������ֿռ�����
	void XUI_EditBox::RenderSelf(const CRect& clipper)
	{
		CPoint pt = m_wndRect.TopLeft();
		AdjustPoint( pt, true );

		XUI_DrawRect( m_wndRect, 0xff00ff00, 0xff00ff00 );

		CRect rc;
		rc.IntersectRect( m_wndRect, clipper );

		CPoint CharPos = pt;
		XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();

		Position begin = m_FirstLineNumber?m_LineRecorder[m_FirstLineNumber-1]:0;
		Position drawline = m_FirstLineNumber;
		for( size_t i = begin; i < m_strText.length()+1; ++i )
		{
			TCHAR c = m_strText[i];

			if( CharPos.y > rc.bottom - pFont->GetCharacterHeight() )
			{
				// �����߶�������
				break;
			}

			if( pFont->GetCharacterWidth( c ) + CharPos.x > rc.right )
			{
				if( m_bWarpText )
				{
					// �ַ���ʾ������ȣ���������ʾ
					if( drawline < m_LineRecorder.size() )
					{
						if( m_LineRecorder[drawline] != i )
						{
							m_LineRecorder.insert( m_LineRecorder.begin() + drawline, i );
							SetCurLineNumber( m_nCurLineNumber + 1 );
						}
						++drawline;
					}
					else
					{
						m_LineRecorder.push_back( i );
						SetCurLineNumber( m_nCurLineNumber + 1 );
					}
				}
				else
				{
					i = drawline?m_LineRecorder[drawline-1]:m_strText.length();
					CharPos.x = pt.x;
				}
			}

			if( drawline != m_FirstLineNumber && (drawline>0?m_LineRecorder[drawline-1]:0) == i )
			{
				// λ��Ϊ���ף������С�
				CharPos.x = pt.x;
				CharPos.y += pFont->GetCharacterHeight();
			}

			BOOL bRender = rc.PtInRect( CharPos + CPoint( pFont->GetCharacterWidth( c ), pFont->GetCharacterHeight() ) );
			if( _istprint( c ) )
			{
				RenderCharacter( c, pFont, CharPos.x, CharPos.y, bRender );
			}
			else
			{
				switch( c )
				{
				case '\t':
					{
						size_t CharCount = i - begin;
						size_t NextTab = CharPos.x + (4 - CharCount%4)*pFont->GetCharacterWidth( _T(' ') );
						if( (long)NextTab > pt.x + m_wndRect.Width() ) NextTab = pt.x + m_wndRect.Width();
						while( CharPos.x < (long)NextTab )
						{
							RenderCharacter( _T(' '), pFont, CharPos.x, CharPos.y, bRender );
						}
					}
					break;
				case '\n':
					++drawline;
				case '\0':
					RenderCharacter( _T(' '), pFont, CharPos.x, CharPos.y, bRender );
					break;
				default:
					RenderCharacter( _T('?'), pFont, CharPos.x, CharPos.y, bRender );
					break;
				}
			}

			if( i == m_CaratPos && m_bShowCarat )
			{
				long x = CharPos.x - long( pFont->GetCharacterWidth( _T('|') )*1.5f );
				long y = CharPos.y;
				RenderCharacter( _T('|'), pFont, x, y, bRender );
			}
		}
	}

	//���
	//����ƶ��¼�
	//����˵����
	//pt���������꣬����ڿؼ�
	//sysKeys��������Ҫ������״̬���μ�MSDN	
	bool XUI_EditBox::onMouseMove(const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	bool XUI_EditBox::onMouseHover(const CPoint& pt)
	{
		return true;
	}

	bool XUI_EditBox::onMouseEnter()
	{
		return true;
	}

	bool XUI_EditBox::onMouseLeave()
	{
		return true;
	}

	//��갴���¼�
	//����˵����
	//button�����µļ���0-�����1-�Ҽ���2-�м�
	//pt����������
	//sysKeys��������Ҫ������״̬���μ�MSDN
	bool XUI_EditBox::onButtonDown(int button, const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	bool XUI_EditBox::onButtonUp(int button, const CPoint& pt, UINT sysKeys)
	{
		return true;
	}

	//����
	//����˵��
	//keycode�����µļ�
	//sysKeys��������Ҫ������״̬���μ�MSDN
	bool XUI_EditBox::onKeyDown(DWORD keycode, UINT sysKeys)
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

	void XUI_EditBox::DeleteCharacter( size_t nPos )
	{
		// �ӵ�һ���ɼ��п�ʼ����
		if( m_nCurLineNumber > 0 )
		{
			line_recorder::iterator i = m_LineRecorder.begin() + m_nCurLineNumber - 1;
			if( *i == nPos+1 )
			{
				m_LineRecorder.erase( i );
				SetCurLineNumber( m_nCurLineNumber - 1 );
			}
		}
		m_strText.erase( nPos, 1 );
	}

	void XUI_EditBox::HandleBack( UINT nSysKey )
	{
		if( m_CaratPos <= 0 ) return;
		DeleteCharacter( --m_CaratPos );
	}

	void XUI_EditBox::HandleDelete( UINT nSysKey )
	{
		if( m_CaratPos >= m_strText.length() ) return;
		DeleteCharacter( m_CaratPos );
	}

	void XUI_EditBox::HandleHome( UINT nSysKey )
	{
		m_CaratPos = m_nCurLineNumber > 0?m_LineRecorder[m_nCurLineNumber-1]:0;
	}

	void XUI_EditBox::HandleEnd( UINT nSysKey )
	{
		m_CaratPos = m_nCurLineNumber < m_LineRecorder.size()?m_LineRecorder[m_nCurLineNumber]-1:m_strText.length();
	}

	void XUI_EditBox::HandleWordLeft( UINT nSysKey )
	{
		while( m_CaratPos > 0 && m_strText[m_CaratPos] != _T(' ') )
		{
			--m_CaratPos;
			if( m_nCurLineNumber > 0 && m_CaratPos < m_LineRecorder[m_nCurLineNumber-1] )
			{
				SetCurLineNumber( m_nCurLineNumber - 1 );
			}
		}
	}

	void XUI_EditBox::HandleCharLeft( UINT nSysKey )
	{
		if( m_CaratPos > 0 )
		{
			--m_CaratPos; 
			if( m_nCurLineNumber > 0 && m_CaratPos < m_LineRecorder[m_nCurLineNumber-1] )
			{
				SetCurLineNumber( m_nCurLineNumber - 1 );
			}
		}
	}

	void XUI_EditBox::HandleWordRight( UINT nSysKey )
	{
		while( m_CaratPos < m_strText.length() && m_strText[m_CaratPos] != _T(' ') )
		{
			++m_CaratPos;
			if( m_nCurLineNumber < m_LineRecorder.size() && m_CaratPos >= m_LineRecorder[m_nCurLineNumber] )
			{
				SetCurLineNumber( m_nCurLineNumber + 1 );
			}
		}
	}

	void XUI_EditBox::HandleCharRight( UINT nSysKey )
	{
		if( m_CaratPos < m_strText.length() )
		{
			++m_CaratPos;
			if( m_nCurLineNumber < m_LineRecorder.size() && m_CaratPos >= m_LineRecorder[m_nCurLineNumber] )
			{
				SetCurLineNumber( m_nCurLineNumber + 1 );
			}
		}
	}

	void XUI_EditBox::HandleReturn( UINT nSysKey )
	{
		m_LineRecorder.insert( m_LineRecorder.begin() + m_nCurLineNumber, m_CaratPos );
		SetCurLineNumber( m_nCurLineNumber + 1 );
	}

	bool XUI_EditBox::onKeyUp( DWORD keycode, UINT sysKeys )
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
	bool XUI_EditBox::onChar(DWORD c, UINT sysKeys)
	{
		if( _istprint( LOWORD(c) ) )
		{
			m_strText.insert( m_CaratPos++, 1, (wchar_t)c );
		}
		return true;
	}

	//���뷨
	//�μ�MSDN
	bool XUI_EditBox::onImeComp(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	bool XUI_EditBox::onImeEndComp(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	bool XUI_EditBox::onImeNotify(DWORD wParam, DWORD lParam)
	{
		return true;
	}

	unsigned int XUI_EditBox::OnMoveWindow( CRect& rcWindow )
	{
		XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();
		if( pFont )
		{
			m_WindowSize.cx		= m_wndRect.Width()/pFont->GetCharacterWidth( _T(' ') );
			m_WindowSize.cy		= m_wndRect.Height()/pFont->GetCharacterHeight();
		}
		Analyse();
		return 0;
	}

	//---------------------------------------------------------------------//
	// describe	: �����ַ�λ�û���к�
	// nPos		: �ַ�����
	// return	: �к�
	//---------------------------------------------------------------------//
	size_t XUI_EditBox::GetLineFromCharaterPos( size_t nPos )const
	{
		struct _compare
		{
			explicit _compare( size_t _nPos ) 
				: nPos( _nPos )
			{
			}

			bool operator()( size_t nLineIndex )
			{
				return nPos < nLineIndex;
			}

			const size_t nPos;
		};
		
		line_recorder::const_iterator i = std::find_if( m_LineRecorder.begin(), m_LineRecorder.end(), _compare( nPos ) );
		if( i != m_LineRecorder.end() )
		{
			return i - m_LineRecorder.begin();
		}

		return -1;
	}

	//---------------------------------------------------------------------//
	// describe	: ������ǰ����ˢ��LineRecorder����
	//---------------------------------------------------------------------//
	void XUI_EditBox::Analyse()
	{

	}

	//---------------------------------------------------------------------//
	// describe	: ���õ�ǰ��
	// line		: ������
	//---------------------------------------------------------------------//
	void XUI_EditBox::SetCurLineNumber( size_t line )
	{
		m_nCurLineNumber = line;
		if( m_nCurLineNumber < m_FirstLineNumber )
		{
			m_FirstLineNumber = ( m_FirstLineNumber <= (size_t)m_WindowSize.cy?0:(m_FirstLineNumber - m_WindowSize.cy) );
		}
		else if( m_nCurLineNumber - m_FirstLineNumber >= (size_t)m_WindowSize.cy )
		{
			m_FirstLineNumber = m_nCurLineNumber - (size_t)m_WindowSize.cy + 1;
		}
	}
}
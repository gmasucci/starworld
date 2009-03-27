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
			m_WindowSize.cx		= m_WindowRect.Width()/pFont->GetCharacterWidth( _T(' ') );
			m_WindowSize.cy		= m_WindowRect.Height()/pFont->GetCharacterHeight();
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

	void XUI_EditBox::RenderCharacter( _tchar szChar, XUI_IFont* pFont, LONG &x, LONG &y, BOOL bRender )
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
		CPoint pt = m_WindowRect.TopLeft();
		AdjustPoint( pt, true );

		XUI_DrawRect( m_WindowRect, 0xff00ff00, 0xff00ff00 );

		CRect rc;
		rc.IntersectRect( m_WindowRect, clipper );

		CPoint CharPos = pt;
		XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();

		Position begin = m_FirstLineNumber?m_LineRecorder[m_FirstLineNumber-1]:0;
		Position drawline = m_FirstLineNumber;
		for( size_t i = begin; i < m_strText.length()+1; ++i )
		{
			_tchar c = m_strText[i];

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
							// û�����б�־�����һ����
							m_LineRecorder.insert( m_LineRecorder.begin() + drawline, i );
							SetCurLineNumber( m_nCurLineNumber + 1 );
						}
						// ��ǰ������
						++drawline;
					}
					else
					{
						// ���һ�������һ����
						m_LineRecorder.push_back( i );
						SetCurLineNumber( m_nCurLineNumber + 1 );
					}
				}
				else
				{
					// �����д���һ�п�ʼ
					i = drawline?m_LineRecorder[drawline-1]:m_strText.length();
					CharPos.x = pt.x;
				}
			}

			if( drawline != m_FirstLineNumber && (drawline>0?m_LineRecorder[drawline-1]:0) == i )
			{
				// ��һ�������������У���ǰλ���Ƿ����ס�
				// λ��Ϊ���ף������С�
				CharPos.x = pt.x;
				CharPos.y += pFont->GetCharacterHeight();
			}

			// �ж��Ƿ񱻻��ơ�
			BOOL bRender = rc.PtInRect( CharPos + CPoint( pFont->GetCharacterWidth( c ), pFont->GetCharacterHeight() ) );
			if( _istprint( c ) )
			{
				// ����ʾ�ַ�
				RenderCharacter( c, pFont, CharPos.x, CharPos.y, bRender );
			}
			else
			{
				switch( c )
				{
				case '\t':
					{
						// �Ʊ����
						size_t CharCount = i - begin;
						size_t NextTab = CharPos.x + (4 - CharCount%4)*pFont->GetCharacterWidth( _T(' ') );
						if( (long)NextTab > pt.x + m_WindowRect.Width() ) NextTab = pt.x + m_WindowRect.Width();
						while( CharPos.x < (long)NextTab )
						{
							RenderCharacter( _T(' '), pFont, CharPos.x, CharPos.y, bRender );
						}
					}
					break;
				case '\n':
					// ��ǰ����������һ�С�
					++drawline;
				case '\0':
					// Ϊ���ƹ��ռλ�á�
					RenderCharacter( _T(' '), pFont, CharPos.x, CharPos.y, bRender );
					break;
				default:
					RenderCharacter( _T('?'), pFont, CharPos.x, CharPos.y, bRender );
					break;
				}
			}

			if( i == m_CaratPos && m_bShowCarat )
			{
				// �Ƿ���ƹ��
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
	bool XUI_EditBox::onKeyDown(uint32 keycode, UINT sysKeys)
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
			HandleBack();
			break;
		case VK_DELETE:
			HandleDelete();
			break;
		case VK_HOME:
			HandleHome();
			break;
		case VK_END:
			HandleEnd();
			break;
		case VK_LEFT:
			if( m_bControl )
				HandleWordLeft();
			else
				HandleCharLeft();
			break;
		case VK_RIGHT:
			if( m_bControl )
				HandleWordRight();
			else
				HandleCharRight();
			break;
		case VK_UP:
			HandleLineUp();
			break;
		case VK_DOWN:
			HandleLineDown();
			break;
		case VK_RETURN:
			m_strText.insert( m_CaratPos++, 1, _T('\n') );
			HandleReturn();
			break;
		}
		return true;
	}

	void XUI_EditBox::DeleteCharacter( size_t nPos )
	{
		// �ӵ�һ���ɼ��п�ʼ����
		if( m_nCurLineNumber > 0 && m_nCurLineNumber < m_LineRecorder.size() )
		{
			line_recorder::iterator i = m_LineRecorder.begin() + m_nCurLineNumber - 1;
			if( *i == nPos+1 )
			{
				i = m_LineRecorder.erase( i );
				SetCurLineNumber( i - m_LineRecorder.begin() + 1 );
			}
		}
		m_strText.erase( nPos, 1 );
	}

	void XUI_EditBox::HandleBack()
	{
		if( m_CaratPos <= 0 ) return;
		DeleteCharacter( --m_CaratPos );
	}

	void XUI_EditBox::HandleDelete()
	{
		if( m_CaratPos >= m_strText.length() ) return;
		DeleteCharacter( m_CaratPos );
	}

	void XUI_EditBox::HandleHome()
	{
		m_CaratPos = m_nCurLineNumber > 0?m_LineRecorder[m_nCurLineNumber-1]:0;
	}

	void XUI_EditBox::HandleEnd()
	{
		m_CaratPos = m_nCurLineNumber < m_LineRecorder.size()?m_LineRecorder[m_nCurLineNumber]-1:m_strText.length();
	}

	void XUI_EditBox::HandleWordLeft()
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

	void XUI_EditBox::HandleCharLeft()
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

	void XUI_EditBox::HandleWordRight()
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

	void XUI_EditBox::HandleCharRight()
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

	void XUI_EditBox::HandleReturn()
	{
		m_LineRecorder.insert( m_LineRecorder.begin() + m_nCurLineNumber, m_CaratPos );
		SetCurLineNumber( m_nCurLineNumber + 1 );
	}

	void XUI_EditBox::HandleLineUp()
	{
		if( m_nCurLineNumber > 0 )
		{
			XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();

			size_t h = m_LineRecorder[m_nCurLineNumber-1];
			long width = pFont->GetStringSize( m_strText.substr( h, m_CaratPos - h ).c_str() ).cx;
			SetCurLineNumber( m_nCurLineNumber - 1 );
		}
	}

	void XUI_EditBox::HandleLineDown()
	{

	}

	bool XUI_EditBox::onKeyUp( uint32 keycode, UINT sysKeys )
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
	bool XUI_EditBox::onChar(uint32 c, UINT sysKeys)
	{
		if( _istprint( LOWORD(c) ) )
		{
			m_strText.insert( m_CaratPos++, 1, (wchar_t)c );
		}
		return true;
	}

	//���뷨
	//�μ�MSDN
	bool XUI_EditBox::onImeComp(uint32 wParam, uint32 lParam)
	{
		return true;
	}

	bool XUI_EditBox::onImeEndComp(uint32 wParam, uint32 lParam)
	{
		return true;
	}

	bool XUI_EditBox::onImeNotify(uint32 wParam, uint32 lParam)
	{
		return true;
	}

	unsigned int XUI_EditBox::OnMoveWindow( CRect& rcWindow )
	{
		XUI_IFont* pFont = m_pFont?m_pFont:GuiSystem::Instance().GetDefaultFont();
		if( pFont )
		{
			m_WindowSize.cx		= m_WindowRect.Width()/pFont->GetCharacterWidth( _T(' ') );
			m_WindowSize.cy		= m_WindowRect.Height()/pFont->GetCharacterHeight();
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
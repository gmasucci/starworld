#include "GuiHeader.h"
#include "GuiSystem.h"
#include "XUI_Dialog.h"
#include "LuaBindClass.h"

/****************************************************
* CGuiSystem
****************************************************/
namespace UILib
{
	CGuiSystem::CGuiSystem()
	: m_pDesktop(NULL)
	, m_bPointInRoot(false)
	, m_is_edit_mode( false )
	, m_hWnd(NULL)
	, m_bInitialized( FALSE )
	, m_nowtime( 0.0f )
	, m_timer_anchor( 0.0f )
	, m_default_font_ptr( NULL )
	, m_capture_element( NULL )
	, m_mouseover_element( NULL )
	, m_mousedown_element( NULL )
	{
		m_pDesktop = new XUI_Window();
		m_pDesktop->SetID( DEFAULT_DESKTOP );
		m_pDesktop->SetName( _T("root" ) );
		// ��ʼ����ʱ��ϵͳ
		m_timer.initialize( 1024, 4096 );
		RegistDesktop( m_pDesktop );
	}

	CGuiSystem::~CGuiSystem(void)
	{
		XUI_Window* pDesktop = RemoveDesktop( DEFAULT_DESKTOP );
		delete pDesktop;
		m_pDesktop = NULL;
	}

	bool CGuiSystem::Initialize( HWND hWnd, _lpcstr lpszPath )
	{
		if( m_bInitialized )	return TRUE;

		_lpcstr path = _fullpath( NULL, lpszPath, 0 );
		m_resource_path	= path;
		free( (void*)path ); 
		path = NULL;

		// ��ʼ��lua�ű�ϵͳ
		Lua::Instance().Initialize();

		XUI_IME::Initialize();
		m_hWnd = hWnd;

		xgcRect rcWindow;
		GetClientRect( hWnd, rcWindow );
		m_windowsize = rcWindow.Size();


		RECT rect;
		::GetClientRect( hWnd, &rect );

		m_pDesktop->MoveWindow( 0, 0, rect.right - rect.left, rect.bottom - rect.top);
		m_capture_element = m_pDesktop;
		return TRUE;
	}

	void CGuiSystem::Unitialize()
	{
		m_default_font_ptr = NULL;
		m_cursor_ptr = NULL;
	}

	void CGuiSystem::SetEditMode( bool bMode )
	{ 
		m_is_edit_mode = bMode; 
		if( !m_is_edit_mode )
			m_cursor_ptr->SetMouse( XUI_MOUSE_APPSTARTING );
	}

	void CGuiSystem::Render()
	{
		if ( m_pDesktop )
		{
			XUI_SetClipping( 0, 0, m_windowsize.cx, m_windowsize.cy );
			m_pDesktop->Render( xgcRect( 0, 0, m_windowsize.cx, m_windowsize.cy ) );
		}

		if( m_cursor_ptr->IsMouseOver() )
		{
			XUI_SetClipping( 0, 0, m_windowsize.cx, m_windowsize.cy );
			if( m_is_edit_mode )
			{
				float x, y;
				m_cursor_ptr->GetMousePos( &x, &y );
				if( m_cursor_ptr->GetKeyState( XUIK_LBUTTON ) && m_mousedown_element && !m_mousedown_element->GetFlags( XUI_Wnd::FLAGS_EDIT ) )
				{
					float x, y;
					m_cursor_ptr->GetMousePos( &x, &y );
					XUI_DrawRect( xgcRect(m_mousedown, xgcPoint(x,y)), XUI_ARGB(0xff,0,0xff,0), XUI_ARGB(0x55,0,0xff,0) );
				}
				XUI_DrawLine( 0, (float)y, (float)m_windowsize.cx, (float)y, XUI_ARGB(0xff,0,0xff,0) );
				XUI_DrawLine( (float)x, 0, (float)x, (float)m_windowsize.cy, XUI_ARGB(0xff,0,0xff,0) );
			}
			XUI_IME::RenderImeWindow();
			m_cursor_ptr->RenderMouse();
		}

	}

	_uint32 CGuiSystem::DetectHandler( XUI_Wnd* pElement, const xgcPoint &pt )
	{
		if( pElement == NULL ) 
			return -1;

		xgcRect rc = pElement->GetWindowRect();
		xgcPoint ptt( pt );
		pElement->ClientToScreen( rc );

		int i = 0;
		xgcRect rch( -2, -2, 2, 2 );
		rch.OffsetRect( rc.TopLeft() );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( rc.Width()/2, 0 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( rc.Width()/2, 0 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( 0, rc.Height()/2 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( 0, rc.Height()/2 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( -rc.Width()/2, 0 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( -rc.Width()/2, 0 );
		if( rch.PtInRect( pt ) ) return i;
		++i;

		rch.OffsetRect( 0, -rc.Height()/2 );
		if( rch.PtInRect( pt ) ) 
			return i;
		++i;

		return -1;
	}

	void CGuiSystem::Update( float fDelta )
	{
		m_nowtime += fDelta;
		while( m_nowtime - m_timer_anchor >= 0.1f )
		{
			m_timer_anchor += 0.1f;
			m_timer.active();
		}

		if( m_cursor_ptr )
		{
			m_cursor_ptr->UpdateMouse( fDelta );
		}

		SLB::LuaCall< void(float, float) >( Lua::Instance().getState(), "UIUpdateEntry" )( m_nowtime, fDelta );
		if( m_pDesktop )
		{
			m_pDesktop->Update( m_nowtime, fDelta );
		}
	}

	bool CGuiSystem::onMouseMove(XUI_Wnd* pElement, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result )
	{
		xgcPoint pt_old = m_mouse_old;
		m_mouse_old = pt;
		if( pElement == NULL ) return false;
		if( !pElement->IsEnable() )	return false;

		XUI_Wnd *pEnterElement = pElement->FindChildInPoint(pt-pElement->GetWindowPosition());

		if( m_is_edit_mode )
		{
			if( m_mousedown_element && m_mousedown_element->GetFlags(XUI_Wnd::FLAGS_EDIT) && ( sysKeys & MK_LBUTTON ) )
			{
				// ��ǰ��ͣ�����ڲ����б��� �� ��������Ҽ�
				long dx = pt.x - pt_old.x;
				long dy = pt.y - pt_old.y;
				struct move_windows
				{
					int dx, dy, h;
					move_windows( int dx, int dy, int h )
						: dx( dx )
						, dy( dy )
						, h( h )
					{

					}

					void operator()( XUI_Wnd* element )
					{
						const xgcRect& r = element->GetWindowRect();
						switch( h )
						{
						case -1:
							element->MoveWindow( r.left+dx, r.top+dy, r.right+dx, r.bottom+dy );
							break;
						case 0:
							element->MoveWindow( r.left+dx, r.top+dy, r.right, r.bottom );
							break;
						case 1:
							element->MoveWindow( r.left, r.top+dy, r.right, r.bottom );
							break;
						case 2:
							element->MoveWindow( r.left, r.top+dy, r.right+dx, r.bottom );
							break;
						case 3:
							element->MoveWindow( r.left, r.top, r.right+dx, r.bottom );
							break;
						case 4:
							element->MoveWindow( r.left, r.top, r.right+dx, r.bottom+dy );
							break;
						case 5:
							element->MoveWindow( r.left, r.top, r.right, r.bottom+dy );
							break;
						case 6:
							element->MoveWindow( r.left+dx, r.top, r.right, r.bottom+dy );
							break;
						case 7:
							element->MoveWindow( r.left+dx, r.top, r.right, r.bottom );
							break;
						}
					}
				};
				std::for_each( m_capture_list.begin(), m_capture_list.end(), move_windows( dx, dy, m_current_handle ) );
			}
			else if( ( m_current_handle = DetectHandler( m_capture_element, pt ) ) != -1 )
			{
				switch( m_current_handle )
				{
				case 0:
				case 4:
					m_cursor_ptr->SetMouse( XUI_MOUSE_SIZENWSE );
					break;
				case 1:
				case 5:
					m_cursor_ptr->SetMouse( XUI_MOUSE_SIZENS );
					break;
				case 2:
				case 6:
					m_cursor_ptr->SetMouse( XUI_MOUSE_SIZENESW );
					break;
				case 3:
				case 7:
					m_cursor_ptr->SetMouse( XUI_MOUSE_SIZEWE );
					break;
				}
			}
			else if( pEnterElement->GetFlags(XUI_Wnd::FLAGS_EDIT) )
			{
				m_cursor_ptr->SetMouse( XUI_MOUSE_SIZEALL );
			}
			else
			{
				m_cursor_ptr->SetMouse( XUI_MOUSE_ARROW );
			}
		}

		if( pEnterElement && pEnterElement != m_mouseover_element )
		{
			if( m_mouseover_element ) 
				m_mouseover_element->onMouseLeave();
			pEnterElement->onMouseEnter();
			m_mouseover_element = pEnterElement;
		}

		if( m_mouseover_element->onMouseMove(pt, sysKeys) == false )
		{
			*result = m_mouseover_element->SendUIMessage( UIM_MOUSEMOVE, MAKELONG(pt.x,pt.y), sysKeys );
		}
		return false;
	}

	bool CGuiSystem::onButtonDown( XUI_Wnd* pElement, _uint32 nButton, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result )
	{
		if( pElement == NULL )
			return false;

		if( !pElement->IsEnable() )	
			return false;

		m_mousedown = pt;
		m_mousedown_element = m_mouseover_element;
		if( m_is_edit_mode )
		{
			if( !m_mousedown_element->GetFlags( XUI_Wnd::FLAGS_EDIT ) && !(sysKeys & MK_CONTROL) )
			{
				std::for_each( m_capture_list.begin(), m_capture_list.end(), 
					std::bind2nd( 
					std::mem_fun1< void, XUI_Wnd, _uint16 >( &XUI_Wnd::ClrFlags ), XUI_Wnd::FLAGS_EDIT ) );

				m_capture_list.clear();
			}
			m_current_handle = DetectHandler( m_capture_element, pt );
		}
		else
		{
			if( m_capture_element && m_capture_element != m_mouseover_element) 
			{
				m_capture_element->SetFocus(false);
				m_mousedown_element->SetFocus( true );
			}

			if( m_mousedown_element->onButtonDown( nButton, pt, sysKeys ) == false )
			{
				*result = m_mousedown_element->SendUIMessage( UIM_BUTTONDOWN_BEGIN + nButton, MAKELONG(pt.x, pt.y), sysKeys );
			}
		}
		m_capture_element = m_mousedown_element;
		return false;
	}

	bool CGuiSystem::onButtonUp( XUI_Wnd* pElement, _uint32 nButton, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result )
	{
		if( pElement == NULL ) 
			return false;

		if( !pElement->IsEnable() )
			return false;

		if( m_is_edit_mode )
		{
			xgcRect rcArea( m_mousedown, pt );
			if( rcArea.IsRectEmpty() )
			{
				XUI_Wnd* find_element = pElement->FindChildInPoint( pt - pElement->GetWindowPosition() );
				if( find_element && !find_element->GetFlags( XUI_Wnd::FLAGS_EDIT ) )
				{
					if( std::find_if( m_capture_list.begin(), m_capture_list.end(), 
						std::bind2nd( std::equal_to< XUI_Wnd* >(), find_element ) ) == m_capture_list.end() )
					{
						find_element->SetFlags( XUI_Wnd::FLAGS_EDIT );
						m_capture_list.push_back( find_element );
					}
				}
			}
			else
			{
				CWndList capture_list;
				pElement->FindRectIn( rcArea - pElement->GetWindowPosition(), capture_list );

				CWndList::iterator result = capture_list.begin();
				do
				{
					result = std::find_first_of( result, capture_list.end(), m_capture_list.begin(), m_capture_list.end() );
					if( result != capture_list.end() )
						result = capture_list.erase( result );
				}while( result != capture_list.end() );
				m_capture_list.insert( m_capture_list.end(), capture_list.begin(), capture_list.end() );

				std::for_each( m_capture_list.begin(), m_capture_list.end(), 
					std::bind2nd( std::mem_fun1< void, XUI_Wnd, _uint16 >( &XUI_Wnd::SetFlags ), XUI_Wnd::FLAGS_EDIT ) );
			}
		}
		else if( m_capture_element->onButtonUp( nButton, pt, sysKeys ) == false )
		{
			*result = m_capture_element->SendUIMessage( UIM_BUTTONUP_BEGIN + nButton, MAKELONG(pt.x, pt.y), sysKeys );
		}

		return false;
	}

	LRESULT CGuiSystem::HandleMessage( HWND hWnd, _uint32 uMsg, int_ptr wParam, long_ptr lParam )
	{
		bool ret = false;
		long_ptr result = 0;
		switch( uMsg )
		{
		case WM_NCCREATE:
			m_hWnd = hWnd;
			break;
		case WM_SIZE:
			{
				xgcRect rcWindow;
				GetClientRect( m_hWnd, rcWindow );
				m_windowsize = rcWindow.Size();
			}
			break;
		case WM_INPUTLANGCHANGE:
			XUI_IME::OnInputLangChange();
			break;
		case WM_IME_SETCONTEXT:
			lParam = 0;
			break;
		case WM_IME_STARTCOMPOSITION:
			XUI_IME::ResetCompositionString();
			return 0;
			break;
			//���뷨
		case WM_IME_COMPOSITION:
			ret = onImeComp( m_pDesktop, wParam, lParam, &result );
			break;
		case WM_IME_ENDCOMPOSITION:
			ret = onImeEndComp( m_pDesktop, wParam, lParam, &result );
			break;
		case WM_IME_NOTIFY:
			ret = onImeNotify( m_pDesktop, wParam, lParam, &result );
			break;
		default:
			if( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST )
			{
				ret = HandleMouse( uMsg, wParam, lParam, &result );
			}
			else if( uMsg >= WM_KEYFIRST && uMsg <= WM_IME_KEYLAST )
			{
				ret = HandleKeyboard( uMsg, wParam, lParam, &result );
			}
		}

		return ret?result:XUI_DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	//�������
	bool CGuiSystem::HandleMouse(_uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result )
	{
		// �����ģ̬�Ի�������Ϣ������ģ̬�Ի�����
		XUI_Window* pDesktop = m_pDesktop;
		if( !m_ModalList.empty() )
		{
			pDesktop = *m_ModalList.begin();
		}

		bool ret = false;
		if ( pDesktop )
		{
			//��ȡ��������
			xgcPoint pt( LOWORD(lParam), HIWORD(lParam) );

			//�ַ���Ϣ
			switch (uMsg)
			{
			case WM_MOUSEMOVE:
				ret = onMouseMove( pDesktop, pt, wParam, result );
				break;
			case WM_LBUTTONDOWN:
				ret = onButtonDown( pDesktop, XUI_LBUTTON, pt, wParam, result );
				break;
			case WM_RBUTTONDOWN:
				ret = onButtonDown( pDesktop, XUI_RBUTTON, pt, wParam, result );
				break;
			case WM_MBUTTONDOWN:
				ret = onButtonDown( pDesktop, XUI_MBUTTON, pt, wParam, result );
				break;
			case WM_LBUTTONUP:
				ret = onButtonUp( pDesktop, XUI_LBUTTON, pt, wParam, result );
				break;
			case WM_RBUTTONUP:
				ret = onButtonUp( pDesktop, XUI_RBUTTON, pt, wParam, result );
				break;
			case WM_MBUTTONUP:
				ret = onButtonUp( pDesktop, XUI_MBUTTON, pt, wParam, result );
				break;
			}
		}
		return ret;
	}

	bool CGuiSystem::onKeyDown( XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result )
	{
		if( m_capture_element == NULL ) return false;
		if( !m_capture_element->IsEnable() ) return false;
		if( m_capture_element->onKeyDown(dwVirtualCode, sysKeys) )
		{
			*result = m_capture_element->SendUIMessage( UIM_KEYDOWN, dwVirtualCode, sysKeys );
			return true;
		}
		return false;
	}

	bool CGuiSystem::onKeyUp(XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result )
	{
		if( m_capture_element == NULL ) return false;
		if( !m_capture_element->IsEnable() ) return false;
		if( m_capture_element->onKeyUp(dwVirtualCode, sysKeys) )
		{
			*result = pElement->SendUIMessage( UIM_KEYUP, dwVirtualCode, sysKeys );
			return true;
		}
		return false;
	}

	bool CGuiSystem::onChar(XUI_Wnd* pElement, _uint32 dwChar, _uint32 sysKeys, long_ptr *result )
	{
		if( m_capture_element == NULL ) return false;
		if( !m_capture_element->IsEnable() ) return false;
		if( m_capture_element->onChar( dwChar, sysKeys ) )
		{
			*result = pElement->SendUIMessage( UIM_CHAR, dwChar, sysKeys );
			return true;
		}
		return false;
	}

	//�������
	bool CGuiSystem::HandleKeyboard( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result )
	{
		XUI_Window* pDesktop = m_pDesktop;
		if( !m_ModalList.empty() )
		{
			pDesktop = *m_ModalList.begin();
		}

		bool ret = false;
		if ( pDesktop )
		{
			//�ַ���Ϣ
			switch(uMsg)
			{
			case WM_KEYDOWN:
				ret = onKeyDown(pDesktop, wParam, lParam, result );
				break;
			case WM_KEYUP:
				ret = onKeyUp(pDesktop, wParam, lParam, result );
				break;
			case WM_CHAR:
				ret = onChar(pDesktop, wParam, lParam, result );
				break;
			}
		}
		return ret;
	}

	bool CGuiSystem::onImeComp(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
	{
		//if (pElement->m_pChildFocusedOn)
		//	return onImeComp( pElement->m_pChildFocusedOn, wParam, lParam, result );
		//else
		//	return pElement->onImeComp(wParam, lParam);
		return ( m_capture_element != NULL )?m_capture_element->onImeComp( wParam, lParam ):false;
	}

	bool CGuiSystem::onImeEndComp(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
	{
		//if( pElement->m_pChildFocusedOn )
		//	return onImeEndComp( pElement->m_pChildFocusedOn, wParam, lParam, result );
		//else
		//	return pElement->onImeEndComp( wParam, lParam );
		return ( m_capture_element != NULL )?m_capture_element->onImeEndComp( wParam, lParam ):false;
	}

	bool CGuiSystem::onImeNotify(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
	{
		//if (pElement->m_pChildFocusedOn)
		//	return onImeNotify( pElement->m_pChildFocusedOn, wParam, lParam, result );
		//else
		//	return pElement->onImeNotify( wParam, lParam );
		return ( m_capture_element != NULL )?m_capture_element->onImeNotify( wParam, lParam ):false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��������
	bool CGuiSystem::LoadFromFile( _lpcstr pszFilename )
	{
		if( m_pDesktop )
		{
			TiXmlDocument Doc;
			if( Doc.LoadFile( (m_resource_path + pszFilename).c_str() ) == false )
				return false;

			TiXmlNode* pNode = Doc.FirstChild( "WINDOW" );
			if( pNode )
			{
				bool ret = m_pDesktop->CreateFromXMLNode( pNode->ToElement() );
				float x, y;
				m_cursor_ptr->GetMousePos( &x, &y );
				m_capture_element = m_pDesktop->FindChildInPoint( xgcPoint( (long)x, (long)y ) );
				return ret;
			}
		}
		return false;
	}

	bool CGuiSystem::SaveToFile( _lpcstr pszFilename )
	{
		if( m_pDesktop )
		{
			TiXmlDocument	Doc;

			TiXmlElement	XmlElement2( "WINDOW" );
			if( m_pDesktop->SaveToXMLNode( &XmlElement2 ) )
			{
				Doc.InsertEndChild( XmlElement2 );

				Doc.SaveFile( (m_resource_path + pszFilename).c_str() );
				return true;
			}
		}
		return false;
	}

	_lpcstr	CGuiSystem::GetResourcePath()
	{
		return m_resource_path.c_str();
	}

	void CGuiSystem::RegistDesktop( XUI_Window* pDesktop )
	{
		if( !pDesktop )	return;

		CDesktopMap::const_iterator citer = m_DesktopMap.find( pDesktop->GetID() );
		if( citer == m_DesktopMap.end() )
		{
			m_DesktopMap[pDesktop->GetID()] = pDesktop;
		}
	}

	void CGuiSystem::SwitchDesktop( int nDesktopID )
	{
		CDesktopMap::const_iterator citer = m_DesktopMap.find( nDesktopID );
		if( citer != m_DesktopMap.end() )
		{
			m_pDesktop->SendUIMessage( UIM_SWITCHLEAVE, 0, 0 );
			m_pDesktop = citer->second;
			m_pDesktop->SendUIMessage( UIM_SWITCHENTER, 0, 0 );
		}
	}

	XUI_Window* CGuiSystem::RemoveDesktop( int nDesktopID )
	{
		CDesktopMap::iterator iter = m_DesktopMap.find( nDesktopID );
		if( iter != m_DesktopMap.end() )
		{
			XUI_Window* pDesktop = iter->second;
			m_DesktopMap.erase( iter );
			if( nDesktopID != DEFAULT_DESKTOP && m_pDesktop == pDesktop )
			{
				SwitchDesktop( DEFAULT_DESKTOP );
			}
			return pDesktop;
		}
		return NULL;
	}

	XUI_Window* CGuiSystem::GetDesktop( UINT nDesktopID )
	{
		CDesktopMap::iterator iter = m_DesktopMap.find( nDesktopID );
		if( iter != m_DesktopMap.end() )
			return iter->second;
		else
			return NULL;
	}

	bool CGuiSystem::EnterModaless( XUI_Window* pDialog )
	{
		if( pDialog == NULL )	return false;

		m_ModalList.push_front( pDialog );
		return true;
	}

	void CGuiSystem::LeaveModaless()
	{
		m_ModalList.pop_front();
	}

	unsigned int CGuiSystem::SetTimer( TimerFunction function, unsigned short repeat, unsigned short timer )
	{
		return m_timer.insert_event( function, repeat, timer );
	}

	void CGuiSystem::KillTimer( unsigned int handle )
	{
		m_timer.remove_event( handle );
	}

	bool SetupDebuger()
	{
		return Lua::Instance().SetupDebuger();
	}
};

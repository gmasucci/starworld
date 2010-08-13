#include "XUI_Header.h"
#include "XUI_System.h"
#include "XUI_Dialog.h"
#include "XUI_LuaBinder.h"

/****************************************************
* XUI_System
****************************************************/
namespace XGC
{
	namespace ui
	{

		LRESULT WndProc( HWND hWnd, _uint32 uMsg, int_ptr wParam, long_ptr lParam )
		{
			return XUI::Instance().HandleMessage( hWnd, uMsg, wParam, lParam );
		}

		XUI_System::XUI_System()
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
			, m_OldProc(NULL)
		{
			m_pDesktop = new XUI_Window();
			m_pDesktop->SetName( _T("root" ) );
			// 初始化定时器系统
			m_timer.initialize( 1024, 4096 );
			RegistDesktop( m_pDesktop );
		}

		XUI_System::~XUI_System(void)
		{
			XUI_Window* pDesktop = RemoveDesktop( DEFAULT_DESKTOP );
			delete pDesktop;
			m_pDesktop = NULL;
		}

		bool XUI_System::Initialize( HWND hWnd, _lpcstr lpszPath )
		{
			if( m_bInitialized )	return TRUE;

			_lpcstr path = _fullpath( NULL, lpszPath, 0 );
			m_resource_path	= path;
			free( (void*)path ); 
			path = NULL;

			// 初始化lua脚本系统
			Lua::Instance().Initialize();

			XUI_IME::Initialize();
			m_hWnd = hWnd;

			m_OldProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WndProc);
			RECT rcWindow;
			GetClientRect( hWnd, &rcWindow );
			m_WindowSize.cx = rcWindow.right - rcWindow.left;
			m_WindowSize.cy = rcWindow.bottom - rcWindow.top;

			m_pDesktop->MoveWindow( 0, 0, m_WindowSize.cx, m_WindowSize.cy );
			m_capture_element = m_pDesktop;
			return TRUE;
		}

		void XUI_System::Unitialize()
		{
			m_default_font_ptr = NULL;
			m_pInput = NULL;
		}

		void XUI_System::SetEditMode( bool bMode )
		{ 
			m_is_edit_mode = bMode; 
			if( !m_is_edit_mode )
				m_pInput->SetMouse( XUI_MOUSE_APPSTARTING );
		}

		void XUI_System::Render()
		{
			if ( m_pDesktop )
			{
				XUI_SetClipping( 0, 0, m_WindowSize.cx, m_WindowSize.cy );
				m_pDesktop->Render( iRect( 0, 0, m_WindowSize.cx, m_WindowSize.cy ) );
			}

			if( m_pInput->IsMouseOver() )
			{
				XUI_SetClipping( 0, 0, m_WindowSize.cx, m_WindowSize.cy );
				if( m_is_edit_mode )
				{
					int x, y;
					m_pInput->GetMousePos( &x, &y );
					if( m_pInput->GetKeyState( XUIK_LBUTTON ) && m_mousedown_element && !m_mousedown_element->GetFlags( XUI_Wnd::FLAGS_EDIT ) )
					{
						int x, y;
						m_pInput->GetMousePos( &x, &y );
						XUI_DrawRect( iRect(m_mousedown, iPoint(x,y)), XUI_ARGB(0xff,0,0xff,0), XUI_ARGB(0x55,0,0xff,0) );
					}
					XUI_DrawLine( 0, (float)y, (float)m_WindowSize.cx, (float)y, XUI_ARGB(0xff,0,0xff,0) );
					XUI_DrawLine( (float)x, 0, (float)x, (float)m_WindowSize.cy, XUI_ARGB(0xff,0,0xff,0) );
				}
				XUI_IME::RenderImeWindow();
				m_pInput->RenderMouse();
			}

		}

		_uint32 XUI_System::DetectHandler( XUI_Wnd* pElement, const iPoint &pt )
		{
			if( pElement == NULL ) 
				return -1;

			iRect rc = pElement->GetWindowRect();
			iPoint ptt( pt );
			pElement->ClientToScreen( rc );

			int i = 0;
			iRect rch( -2, -2, 2, 2 );
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

		void XUI_System::Update( float fDelta )
		{
			m_nowtime += fDelta;
			while( m_nowtime - m_timer_anchor >= 0.1f )
			{
				m_timer_anchor += 0.1f;
				m_timer.active();
			}

			if( m_pInput )
			{
				m_pInput->UpdateMouse( fDelta );
			}

			SLB::LuaCall< void(float, float) >( Lua::Instance().getState(), "UIUpdateEntry" )( m_nowtime, fDelta );
			if( m_pDesktop )
			{
				m_pDesktop->Update( m_nowtime, fDelta );
			}
		}

		bool XUI_System::OnMouseMove(XUI_Wnd* pElement, const iPoint& pt, _uint32 sysKeys, long_ptr *result )
		{
			iPoint pt_old = m_mouse_old;
			m_mouse_old = pt;
			if( pElement == NULL ) return false;
			if( !pElement->IsEnable() )	return false;

			XUI_Wnd *pEnterElement = pElement->FindChildInPoint(pt-pElement->GetWindowPosition());

			if( m_is_edit_mode )
			{
				if( m_mousedown_element && m_mousedown_element->GetFlags(XUI_Wnd::FLAGS_EDIT) && ( sysKeys & MK_LBUTTON ) )
				{
					// 当前悬停对象在捕获列表中 且 按下鼠标右键
					int dx = pt.x - pt_old.x;
					int dy = pt.y - pt_old.y;
					struct move_windows
					{
						int dx, dy;
						int h;
						move_windows( int dx, int dy, int h )
							: dx( dx )
							, dy( dy )
							, h( h )
						{

						}

						void operator()( XUI_Wnd* element )
						{
							const iRect& r = element->GetWindowRect();
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
						m_pInput->SetMouse( XUI_MOUSE_SIZENWSE );
						break;
					case 1:
					case 5:
						m_pInput->SetMouse( XUI_MOUSE_SIZENS );
						break;
					case 2:
					case 6:
						m_pInput->SetMouse( XUI_MOUSE_SIZENESW );
						break;
					case 3:
					case 7:
						m_pInput->SetMouse( XUI_MOUSE_SIZEWE );
						break;
					}
				}
				else if( pEnterElement->GetFlags(XUI_Wnd::FLAGS_EDIT) )
				{
					m_pInput->SetMouse( XUI_MOUSE_SIZEALL );
				}
				else
				{
					m_pInput->SetMouse( XUI_MOUSE_ARROW );
				}
			}

			if( pEnterElement && pEnterElement != m_mouseover_element )
			{
				if( m_mouseover_element ) 
					m_mouseover_element->OnMouseLeave();
				pEnterElement->OnMouseEnter();
				m_mouseover_element = pEnterElement;
			}

			m_mouseover_element->OnMouseMove(pt, sysKeys);
			return false;
		}

		bool XUI_System::OnButtonDown( XUI_Wnd* pElement, _uint32 nButton, const iPoint& pt, _uint32 sysKeys, long_ptr *result )
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

				m_mousedown_element->OnButtonDown( nButton, pt, sysKeys );
			}
			m_capture_element = m_mousedown_element;
			return false;
		}

		bool XUI_System::OnButtonUp( XUI_Wnd* pElement, _uint32 nButton, const iPoint& pt, _uint32 sysKeys, long_ptr *result )
		{
			if( pElement == NULL ) 
				return false;

			if( !pElement->IsEnable() )
				return false;

			if( m_is_edit_mode )
			{
				iRect rcArea( m_mousedown, pt );
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
			m_capture_element->OnButtonUp( nButton, pt, sysKeys );
			return false;
		}

		LRESULT XUI_System::HandleMessage( HWND hWnd, _uint32 uMsg, int_ptr wParam, long_ptr lParam )
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
					RECT rcWindow;
					GetClientRect( m_hWnd, &rcWindow );
					m_WindowSize = iSize( rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top );
				}
				break;
			case WM_INPUTLANGCHANGE:
				XUI_IME::OnInputLangChange();
				ret = true;
				break;
			case WM_IME_SETCONTEXT:
				lParam = 0;
				break;
			case WM_IME_STARTCOMPOSITION:
				XUI_IME::ResetCompositionString();
				ret = true;
				break;
				//输入法
			case WM_IME_COMPOSITION:
				ret = OnImeComp( m_pDesktop, wParam, lParam, &result );
				break;
			case WM_IME_ENDCOMPOSITION:
				ret = OnImeEndComp( m_pDesktop, wParam, lParam, &result );
				break;
			case WM_IME_NOTIFY:
				ret = OnImeNotify( m_pDesktop, wParam, lParam, &result );
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

			return ret?result:CallWindowProc( m_OldProc, hWnd, uMsg, wParam, lParam );
		}

		//处理鼠标
		bool XUI_System::HandleMouse(_uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result )
		{
			// 如果有模态对话框则将消息都发往模态对话框中
			XUI_Window* pDesktop = m_pDesktop;
			if( !m_ModalList.empty() )
			{
				pDesktop = *m_ModalList.begin();
			}

			bool ret = false;
			if ( pDesktop )
			{
				//获取鼠标的坐标
				iPoint pt( LOWORD(lParam), HIWORD(lParam) );

				//分发消息
				switch (uMsg)
				{
				case WM_MOUSEMOVE:
					ret = OnMouseMove( pDesktop, pt, wParam, result );
					break;
				case WM_LBUTTONDOWN:
					ret = OnButtonDown( pDesktop, XUI_LBUTTON, pt, wParam, result );
					break;
				case WM_RBUTTONDOWN:
					ret = OnButtonDown( pDesktop, XUI_RBUTTON, pt, wParam, result );
					break;
				case WM_MBUTTONDOWN:
					ret = OnButtonDown( pDesktop, XUI_MBUTTON, pt, wParam, result );
					break;
				case WM_LBUTTONUP:
					ret = OnButtonUp( pDesktop, XUI_LBUTTON, pt, wParam, result );
					break;
				case WM_RBUTTONUP:
					ret = OnButtonUp( pDesktop, XUI_RBUTTON, pt, wParam, result );
					break;
				case WM_MBUTTONUP:
					ret = OnButtonUp( pDesktop, XUI_MBUTTON, pt, wParam, result );
					break;
				}
			}
			return ret;
		}

		bool XUI_System::OnKeyDown( XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result )
		{
			if( m_capture_element == NULL ) return false;
			if( !m_capture_element->IsEnable() ) return false;
			if( m_capture_element->OnKeyDown(dwVirtualCode, sysKeys) )
			{
				return true;
			}
			return false;
		}

		bool XUI_System::OnKeyUp(XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result )
		{
			if( m_capture_element == NULL ) return false;
			if( !m_capture_element->IsEnable() ) return false;
			if( m_capture_element->OnKeyUp(dwVirtualCode, sysKeys) )
			{
				return true;
			}
			return false;
		}

		bool XUI_System::OnChar(XUI_Wnd* pElement, _uint32 dwChar, _uint32 sysKeys, long_ptr *result )
		{
			if( m_capture_element == NULL ) return false;
			if( !m_capture_element->IsEnable() ) return false;
			if( m_capture_element->OnChar( dwChar, sysKeys ) )
			{
				return true;
			}
			return false;
		}

		//处理键盘
		bool XUI_System::HandleKeyboard( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result )
		{
			XUI_Window* pDesktop = m_pDesktop;
			if( !m_ModalList.empty() )
			{
				pDesktop = *m_ModalList.begin();
			}

			bool ret = false;
			if ( pDesktop )
			{
				//分发消息
				switch(uMsg)
				{
				case WM_KEYDOWN:
					ret = OnKeyDown(pDesktop, wParam, lParam, result );
					break;
				case WM_KEYUP:
					ret = OnKeyUp(pDesktop, wParam, lParam, result );
					break;
				case WM_CHAR:
					ret = OnChar(pDesktop, wParam, lParam, result );
					break;
				}
			}
			return ret;
		}

		bool XUI_System::OnImeComp(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
		{
			//if (pElement->m_pChildFocusedOn)
			//	return onImeComp( pElement->m_pChildFocusedOn, wParam, lParam, result );
			//else
			//	return pElement->onImeComp(wParam, lParam);
			return ( m_capture_element != NULL )?m_capture_element->OnImeComp( wParam, lParam ):false;
		}

		bool XUI_System::OnImeEndComp(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
		{
			//if( pElement->m_pChildFocusedOn )
			//	return onImeEndComp( pElement->m_pChildFocusedOn, wParam, lParam, result );
			//else
			//	return pElement->onImeEndComp( wParam, lParam );
			return ( m_capture_element != NULL )?m_capture_element->OnImeEndComp( wParam, lParam ):false;
		}

		bool XUI_System::OnImeNotify(XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result )
		{
			//if (pElement->m_pChildFocusedOn)
			//	return onImeNotify( pElement->m_pChildFocusedOn, wParam, lParam, result );
			//else
			//	return pElement->onImeNotify( wParam, lParam );
			return ( m_capture_element != NULL )?m_capture_element->OnImeNotify( wParam, lParam ):false;
		}

		//////////////////////////////////////////////////////////////////////////
		// 界面生成
		bool XUI_System::LoadFromFile( _lpcstr pszFilename )
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
					int x, y;
					m_pInput->GetMousePos( &x, &y );
					m_capture_element = m_pDesktop->FindChildInPoint( iPoint( x, y ) );
					return ret;
				}
			}
			return false;
		}

		bool XUI_System::SaveToFile( _lpcstr pszFilename )
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

		_lpcstr	XUI_System::GetResourcePath()
		{
			return m_resource_path.c_str();
		}

		void XUI_System::RegistDesktop( XUI_Window* pDesktop )
		{
			if( !pDesktop )	return;

			CDesktopMap::const_iterator citer = m_DesktopMap.find( pDesktop->GetID() );
			if( citer == m_DesktopMap.end() )
			{
				m_DesktopMap[pDesktop->GetID()] = pDesktop;
			}
		}

		void XUI_System::SwitchDesktop( int nDesktopID )
		{
			CDesktopMap::const_iterator citer = m_DesktopMap.find( nDesktopID );
			if( citer != m_DesktopMap.end() )
			{
				m_pDesktop = citer->second;
			}
		}

		XUI_Window* XUI_System::RemoveDesktop( int nDesktopID )
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

		XUI_Window* XUI_System::GetDesktop( UINT nDesktopID )
		{
			CDesktopMap::iterator iter = m_DesktopMap.find( nDesktopID );
			if( iter != m_DesktopMap.end() )
				return iter->second;
			else
				return NULL;
		}

		bool XUI_System::EnterModaless( XUI_Window* pDialog )
		{
			if( pDialog == NULL )	return false;

			m_ModalList.push_front( pDialog );
			return true;
		}

		void XUI_System::LeaveModaless()
		{
			m_ModalList.pop_front();
		}

		unsigned int XUI_System::SetTimer( _tfunction function, unsigned short repeat, unsigned short timer )
		{
			return m_timer.insert_event( function, repeat, timer );
		}

		void XUI_System::KillTimer( unsigned int handle )
		{
			m_timer.remove_event( handle );
		}

		bool SetupDebuger()
		{
			return Lua::Instance().SetupDebuger();
		}
	}
};

#include "GuiHeader.h"
#include "GuiSystem.h"
#include "XUI_Dialog.h"

namespace UILib
{
//////////////////////////////////////////////////////////////////////////
// Dialog class
//////////////////////////////////////////////////////////////////////////
	BEGIN_UIMSG_MAP( CXDialog, XUI_Window )
	END_UIMSG_MAP()
	CXDialog::CXDialog( _lpctstr lpszTemplate /*= NULL*/ )
	{
		m_bTranslateParent = false;
		m_bEnableParent = false;
		m_bModal = false;
		m_nResult = 0;
		if( lpszTemplate )	m_strTemplate = lpszTemplate;
		//BindProperty( _T("Template"), m_strTemplate );
	}

	CXDialog::~CXDialog()
	{
		OnDestroy();
	}

	bool CXDialog::Create( _lpctstr strTemplate, XUI_Window* pParent /* = NULL  */)
	{
		m_strTemplate = strTemplate;
		if( pParent )
		{
			SetParent( pParent );
			pParent->AddChild( this );
		}

		TiXmlDocument Doc;
		USES_CONVERSION;
		char* p = T2A( m_strTemplate.c_str() );
		if( Doc.LoadFile( p ) )
		{
			TiXmlElement* pElement = Doc.FirstChildElement( "DESKTOP" );
			CreateFromXMLNode( pElement );
			x_rect rc;

			rc.left		= pElement->IntAttribute( "X" );
			rc.top		= pElement->IntAttribute( "Y" );
			rc.right	= pElement->IntAttribute( "Width" ) + rc.left;
			rc.bottom	= pElement->IntAttribute( "Height" ) + rc.top;

			MoveWindow( rc.left, rc.top, rc.right, rc.bottom );

			ShowWindow( true );
			return OnInitDialog();
		}
		else
		{
			ASSERT( 0 );
		}
		return false;
	}

	XUI_Wnd* CXDialog::PreModal()
	{
		XUI_Wnd* pParent = m_pParent;
		if( pParent == NULL )
		{
			//_assert( _afxCurrentGuiSystem );
			pParent = GuiSystem::Instance().GetRoot();
		}

		while( pParent->GetParent() )
		{
			pParent = pParent->GetParent();
		}
		pParent->EnableWindow( false );
		m_nResult = 0;
		m_bEnableParent = true;
		m_bModal = true;
		GuiSystem::Instance().EnterModaless( this );
		return pParent;
	}

	UINT CXDialog::DoModal( fnModalMsgProc pfn )
	{
		XUI_Wnd* pParent = PreModal();
		//ASSERT( pParent );
		if( !Create( m_strTemplate.c_str(), (XUI_Window*)pParent ) )
		{
			ASSERT( FALSE );
			return -1;
		}

		BeginModalLoop( pfn );
		Release();
		PostModal();
		if( m_bEnableParent )
		{
			pParent->EnableWindow( true );
			m_bEnableParent = false;
		}

		return m_nResult;
	}

	void CXDialog::BeginModalLoop( fnModalMsgProc pfn )
	{
		for(;;)
		{
			MSG msg;
			while( !::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) )
			{
				pfn( TRUE );
				if( !m_bModal ) return;
			}

			while( ::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) )
			{
				if( !pfn( FALSE ) )
				{
					::PostMessage( GuiSystem::Instance().GetHWND(), WM_QUIT, 0, 0 );
					return;
				}

				if( !m_bModal ) return;
			}
		}
	}

	void CXDialog::PostModal()
	{
		GuiSystem::Instance().LeaveModaless();
	}

	void CXDialog::EndModalLoop()
	{
		m_bModal = false;
	}

	void CXDialog::OnOK()
	{
		//UpdateData( TRUE );
		m_nResult = IDOK;
		EndModalLoop();
	}

	void CXDialog::OnCancel()
	{
		m_nResult = IDCANCEL;
		EndModalLoop();
	}

	void CXDialog::CenterWindow()
	{
		if( m_pParent )
		{
			x_rect rc = m_pParent->GetWindowRect();
			int nOffsetX = rc.Width()/2 - GetWindowRect().Width()/2;
			int nOffsetY = rc.Height()/2 - GetWindowRect().Height()/2;
			Offset( nOffsetX, nOffsetY );
		}
	}

	void CXDialog::RenderSelf()
	{
		XUI_Window::RenderSelf();
	}

	bool CXDialog::onKeyUp(uint32 keycode, UINT sysKeys)
	{
		// 通过Tab键在控件中切换
		if( m_bFocused && keycode == VK_TAB )
		{
			for( size_t i= 0; i < m_pChildren.size(); ++i )
			{
				if( m_pChildFocusedOn == m_pChildren[i] )
				{
					m_pChildFocusedOn->SetFocus( false );
					if( i >= m_pChildren.size() - 1 )
					{
						m_pChildFocusedOn = m_pChildren[0];
					}
					else
					{
						m_pChildFocusedOn = m_pChildren[i+1];
					}
					m_pChildFocusedOn->SetFocus( true );
					break;
				}
			}
		}
		return true;
	}

	void CXDialog::SetFocus( XUI_Wnd* pFocus )
	{
		assert( pFocus );
		if( !pFocus )	return;

		m_pChildFocusedOn->SetFocus( false );
		m_pChildFocusedOn = pFocus;
		pFocus->SetFocus( true );
	}

	void CXDialog::SetFocus( UINT nCtrlID )
	{
		XUI_Wnd* pFocus = FindChildByID( nCtrlID );
		SetFocus( pFocus );
	}

	LRESULT CXDialog::DefMsgProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
	{
		return OnWndMsg( nMsg, wParam, lParam );
	}
}
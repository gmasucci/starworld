#include "GuiHeader.h"
#include "XUI_Wnd.h"
#include "tinyxml.h"
#include "GuiSystem.h"
#include "LuaBindClass.h"
#include "SLB\SLB.hpp"
using namespace SLB;

namespace UILib
{
	/*******************************************************
	XUI_Wnd
	*******************************************************/
	XUI_Wnd::XUI_Wnd(void)
	: m_pParent(NULL)
	, m_bFocused(false)
	, m_bVisible( true )
	, m_bEnable( true )
	, m_bOwnerDraw( false )
	, m_pFont( NULL )
	, m_pBackGround( NULL )
	, m_WindowPosition( 0, 0 )
	, m_WindowSize( 100, 100 )
	{
		m_bTranslateParent = true;
		m_fZ=0.1f;
	}

	XUI_Wnd::~XUI_Wnd(void)
	{
		Destroy();
	}

	void XUI_Wnd::Destroy()
	{
		DestroyAllChild();
		if( m_pParent )
		{
			XUI_Wnd* pParent = m_pParent;
			SetParent( NULL );
			pParent->RemoveChild( this, false );
		}
		SAFE_DELETE( m_pBackGround );
	}

	void XUI_Wnd::Release()
	{
		delete this;
	}

	//�¼�����
	//���
	//���� true �򸸿ؼ����������������Ϣ
	bool XUI_Wnd::onMouseMove(const xgcPoint& pt, _uint32 sysKeys)
	{
		return false;
	}

	bool XUI_Wnd::onMouseEnter()
	{
		return false;
	}

	bool XUI_Wnd::onMouseLeave()
	{
		return false;
	}

	bool XUI_Wnd::onButtonDown(int button, const xgcPoint& pt, _uint32 sysKeys)
	{
		//_uint32 id=0;
		//switch (button)
		//{
		//case 0:
		//	id=EV_MOUSELBTDOWN;
		//	SendUIMessage( EV_MOUSELBTDOWN, sysKeys, MAKELONG( pt.x, pt.y ) );
		//	break;
		//case 1:
		//	id=EV_MOUSERBTDOWN;
		//	SendUIMessage( EV_MOUSERBTDOWN, sysKeys, MAKELONG( pt.x, pt.y ) );
		//	break;
		//case 2:
		//	id=EV_MOUSEMBTDOWN;
		//	break;
		//}
		return false;
	}

	bool XUI_Wnd::onButtonUp(int button, const xgcPoint& pt, _uint32 sysKeys)
	{
		//_uint32 id=0;
		//switch (button)
		//{
		//case 0:
		//	id=EV_MOUSELBTUP;
		//	SendUIMessage( EV_MOUSELBTUP, sysKeys, MAKELONG( pt.x, pt.y ) );
		//	break;
		//case 1:
		//	id=EV_MOUSERBTUP;
		//	SendUIMessage( EV_MOUSERBTUP, sysKeys, MAKELONG( pt.x, pt.y ) );
		//	break;
		//case 2:
		//	id=EV_MOUSEMBTUP;
		//	break;
		//}
		return false;
	}

	//����
	bool XUI_Wnd::onKeyDown(_uint32 keycode, _uint32 sysKeys)
	{
		return false;
	}

	bool XUI_Wnd::onKeyUp(_uint32 keycode, _uint32 sysKeys)
	{
		return false;
	}

	bool XUI_Wnd::onChar(_uint32 c, _uint32 sysKeys)
	{
		return false;
	}

	//���뷨
	bool XUI_Wnd::onImeComp(_uint32 wParam, _uint32 lParam)
	{
		return false;
	}

	bool XUI_Wnd::onImeEndComp(_uint32 wParam, _uint32 lParam)
	{
		return false;
	}

	bool XUI_Wnd::onImeNotify(_uint32 wParam, _uint32 lParam)
	{
		return false;
	}

	//end of �¼�����

	//��һ������ӿؼ�����ת������Ļ����
	void XUI_Wnd::ClientToScreen( xgcPoint& pt )const
	{
		AdjustPoint(pt, true);
		if (m_pParent)
		{
			pt += m_pParent->GetWindowRect().TopLeft();
			return m_pParent->ClientToScreen( pt );
		}
	}

	void XUI_Wnd::ClientToScreen( xgcRect& rc )const
	{
		AdjustWindow( rc, true);
		if( m_pParent )
		{
			rc += m_pParent->GetWindowRect().TopLeft();
			return m_pParent->ClientToScreen( rc );
		}
	}

	//��һ���������Ļ����ת���ɿؼ�����
	void XUI_Wnd::ScreenToClient( xgcPoint& pt )const
	{
		AdjustPoint( pt, false );
		if( m_pParent )
		{
			m_pParent->ScreenToClient( pt );
			pt -= m_pParent->GetWindowRect().TopLeft();
		}
	}

	void XUI_Wnd::ScreenToClient( xgcRect& rc )const
	{
		AdjustWindow( rc, false );
		if( m_pParent )
		{
			m_pParent->ScreenToClient( rc );
			rc -= m_pParent->GetWindowRect().TopLeft();
		}
	}

	int XUI_Wnd::FindChild(XUI_Wnd* pElement) const
	{
		for( _uint32 i = 0; i < m_pChildren.size(); i++ )
			if( m_pChildren[i] == pElement )
				return i;
		return -1;
	}

	void XUI_Wnd::AddChild(XUI_Wnd* pElement )
	{
		if (-1==FindChild(pElement))
		{
			pElement->m_pParent=this;
			m_pChildren.push_back(pElement);
			// ���Լ���Ϊ��Ԫ�صĹ۲���,�������ӿؼ���������Ϣ�������ھͿ���ͨ��
			// ��Ϣӳ���ػ�
			pElement->AddCommonListener( this );
		}
	}

	//���ؼ�����Ϊ�ӿؼ������޸�������
	void XUI_Wnd::AddChildAt(XUI_Wnd* pElement, int x, int y)
	{
		if (pElement)
		{
			if (pElement->m_WindowPosition.x != 0 || pElement->m_WindowPosition.y != 0 )
			{
				pElement->Offset(-pElement->m_WindowPosition.x, -pElement->m_WindowPosition.y );
			}
			pElement->Offset(x, y);
			AddChild(pElement);
		}
	}

	void XUI_Wnd::RemoveChild(XUI_Wnd* pElement, bool bDestroy )
	{
		int i = FindChild( pElement );
		if( i >= 0 )
		{
			std::vector<XUI_Wnd*>::iterator itr = m_pChildren.begin()+i;

			XUI_Wnd* pChild = *itr;
			//if( m_pChildMouseOver == pChild )	m_pChildMouseOver = NULL;
			//if( m_pChildFocusedOn == pChild )	m_pChildFocusedOn = NULL;

			if( bDestroy ) pChild->Release();
			m_pChildren.erase(itr);
		}
	}

	void XUI_Wnd::DestroyAllChild()
	{
		for( size_t i = 0; i < m_pChildren.size(); ++i )
		{
			XUI_Wnd* pXUI_Wnd = m_pChildren[i];
			pXUI_Wnd->SetParent( NULL );
			delete pXUI_Wnd;
		}
		m_pChildren.clear();
		//m_pChildMouseOver = NULL;
		//m_pChildFocusedOn = NULL;
	}

	//Ѱ����ĳ�������ϵĿؼ�
	XUI_Wnd* XUI_Wnd::FindChildInPoint(const xgcPoint &pt, _uint32 *deep )
	{
		xgcPoint adjust( pt );
		AdjustPoint( adjust, true );
		for ( int i=(int)m_pChildren.size()-1; i>=0; i-- )
		{
			XUI_Wnd* pElement = m_pChildren[i];
			if( pElement->m_bVisible && pElement->IsPointIn(adjust) )
				return ((deep&&*deep--)||!deep)?pElement->FindChildInPoint( adjust - m_WindowPosition, deep ):pElement;
		}
		return this;
	}

	//--------------------------------------------------------//
	//	created:	19:11:2009   18:29
	//	filename: 	d:\Develop\StarGame\GameSDK\GameUILib\Source\XUI_Wnd.cpp
	//	author:		Albert.xu
	//
	//	purpose:	�����ܰ����������ε�����Wnd
	//--------------------------------------------------------//
	XUI_Wnd* XUI_Wnd::FindRectIn( const xgcRect &rc )
	{
		xgcRect rcAdjust(rc);
		AdjustWindow( rcAdjust, true);

		xgcRect rcWindow = GetWindowRect();
		if( rcWindow.PtInRect( rc.TopLeft() ) && rcWindow.PtInRect( rc.BottomRight() ) )
		{
			for( int i=(int)m_pChildren.size()-1; i>=0; i-- )
			{
				XUI_Wnd* pElement = m_pChildren[i];
				XUI_Wnd* pRet = pElement->FindRectIn( rcAdjust - m_WindowPosition );
				if( pRet )
					return pRet;
			}
			return this;
		}
		return NULL;
	}

	XUI_Wnd* XUI_Wnd::FindChildByName( const _string& sName) const
	{
		for(size_t i=0; i<m_pChildren.size(); i++)
		{
			XUI_Wnd* pElement=m_pChildren[i];
			if ( !pElement->m_strName.empty() && pElement->m_strName == sName )
				return pElement;
		}
		return NULL;
	}

	XUI_Wnd* XUI_Wnd::FindChildByID( int nID )const
	{
		for(size_t i=0; i<m_pChildren.size(); i++)
		{
			XUI_Wnd* pElement=m_pChildren[i];
			if ( pElement->GetID() == nID )
				return pElement;
		}
		return NULL;
	}

	// �����Ӷ���
	XUI_Wnd* XUI_Wnd::ForAllChild( ElementCallback pfnCallback, void* pData )
	{
		for( size_t i = 0; i < m_pChildren.size(); ++i )
		{
			XUI_Wnd* pElement=m_pChildren[i];
			if( pfnCallback( pElement, pData ) )	return pElement;
		}
		return NULL;
	}

	// �ƶ�����
	void XUI_Wnd::MoveWindow(int left, int top, int right, int bottom, bool notify )
	{
		m_WindowPosition = xgcPoint( left, top );
		m_WindowSize = xgcSize( abs(right - left), abs(bottom - top) );
		if( notify )
		{
			SendUIMessage( UIM_MOVE, 0, MAKELONG( left, top ) );
			SendUIMessage( UIM_SIZE, 0, MAKELONG( m_WindowSize.cx, m_WindowSize.cy ) );
			// ����λ�ñ����Ϣ
			OnMoveWindow( xgcRect( left, top, right, bottom ) );
		}
	}

	void XUI_Wnd::Offset(int x, int y)
	{
		m_WindowPosition.x += x;
		m_WindowPosition.y += y;
		SendUIMessage( WM_MOVE, 0, MAKELONG( m_WindowPosition.x, m_WindowPosition.y ) );
	}

	void XUI_Wnd::ShowWindow( bool bVisible /* = true  */ )
	{
		SendUIMessage( WM_SHOWWINDOW, bVisible, 0 );
		m_bVisible=bVisible;
	}

	void XUI_Wnd::EnableWindow( bool bEnable )
	{
		SendUIMessage( WM_ENABLE, bEnable, 0 );
		m_bEnable = bEnable;
	}

	//--------------------------------------------------------------------------
	void XUI_Wnd::BringToUp()	// ����һ��
	//--------------------------------------------------------------------------
	{
		if( m_pParent )
		{
			int i = m_pParent->FindChild( this );
			if( i > 0 )
				std::swap( m_pParent->m_pChildren[i], m_pParent->m_pChildren[i-1] );
		}
	}

	//--------------------------------------------------------------------------
	void XUI_Wnd::BringToDown() // ����һ��
	//--------------------------------------------------------------------------
	{
		if( m_pParent )
		{
			int i = m_pParent->FindChild( this );
			if( i < ( int )m_pParent->m_pChildren.size() )
				std::swap( m_pParent->m_pChildren[i], m_pParent->m_pChildren[i+1] );
		}
	}

	void XUI_Wnd::BringToFront()
	{
		//���ؼ��Ƶ����
		if (m_pParent)
		{
			int i=m_pParent->FindChild(this);
			if (i>=0)
			{
				_uint32 j;
				for (j=i; j<m_pParent->m_pChildren.size()-1; j++)
					m_pParent->m_pChildren[j]=m_pParent->m_pChildren[j+1];
				m_pParent->m_pChildren[j]=this;
			}
		}
	}

	void XUI_Wnd::BringToEnd()
	{
		if( m_pParent )
		{
			int i = m_pParent->FindChild( this );
			if( i > 0 )
			{
				int j;
				for( j = i; j > 0; --j)
					m_pParent->m_pChildren[j]=m_pParent->m_pChildren[j-1];
				m_pParent->m_pChildren[j]=this;
			}
		}
	}

	//���ÿؼ��Ľ���
	void XUI_Wnd::SetFocus(bool bFocused)
	{
		//�����¼�
		if (bFocused && !m_bFocused)
		{
			SendUIMessage( WM_COMMAND, MAKELONG( GetID(), EV_SETFOCUS ), 0 );
			onGetFocus();
		}
		if (!bFocused && m_bFocused)
		{
			SendUIMessage( WM_COMMAND, MAKELONG( GetID(), EV_KILLFOCUS ), 0 );
			onLostFocus();
		}
		m_bFocused=bFocused;
	}

	BOOL XUI_Wnd::IsPointIn(const xgcPoint& pt)
	{
		return 
			pt.x >= m_WindowPosition.x && 
			pt.x < m_WindowPosition.x + m_WindowSize.cx &&
			pt.y >= m_WindowPosition.y && 
			pt.y < m_WindowPosition.y + m_WindowSize.cy;
	}

	void XUI_Wnd::Render(const xgcRect& clipper)
	{
		if (!m_bVisible)		return;

		//����ɼ�����
		xgcRect clpSelf( GetWindowRect() + clipper.TopLeft() );

		if( clpSelf.IntersectRect( clpSelf, clipper ) )
		{
			//�����Լ�
			if( m_bVisible )
			{
				if( m_bOwnerDraw )
				{
					DRAWSTRUCT ds;
					ds.rcClient		= GetWindowRect();
					ds.rcClipper	= clpSelf;
					ds.pCtrl		= this;
					SendUIMessage( UI_OWNERDRAW, GetID(), (long_ptr)&ds );
				}
				else
				{
					XUI_SetClipping( clpSelf.left, clpSelf.top, clpSelf.Width(), clpSelf.Height() );
					RenderSelf( clipper.TopLeft() );
				}
			}

			//�����ӿؼ�
			for (_uint32 i=0; i<m_pChildren.size(); i++)
			{
				m_pChildren[i]->Render(clpSelf);
			}
		}
	}

	void XUI_Wnd::Update( float timer, float delta )
	{
		try
		{
			LuaCall< void( XUI_Wnd*, float, float ) > LuaUpdate( Lua::Instance(), m_strUpdateFunc.c_str() );
			if( LuaUpdate.valid() )
			{
				LuaUpdate( this, timer, delta );
			}
		}
		catch( std::runtime_error& err )
		{
			puts( err.what() );
		}

		//�����ӿؼ�
		for (_uint32 i=0; i<m_pChildren.size(); i++)
		{
			m_pChildren[i]->Update( timer, delta );
		}
	}

	void XUI_Wnd::RenderSelf( const xgcPoint& adjust )
	{
		if( m_pBackGround )
		{
			XUI_DrawSprite( m_pBackGround, m_WindowPosition.x - adjust.x, m_WindowPosition.y - adjust.y, m_WindowSize.cx, m_WindowSize.cy );
		}
	}

	long_ptr XUI_Wnd::SendUIMessage( _uint32 nMsg, uint_ptr wParam, long_ptr lParam )
	{
		//if( m_pChildFocusedOn )
		//{
		//	return m_pChildFocusedOn->SendUIMessage( nMsg, wParam, lParam );
		//}
		//else 
		//{
		//	return OnWndMsg( nMsg, wParam, lParam );
		//}
		return OnWndMsg( nMsg, wParam, lParam );
	}

	bool XUI_Wnd::save_file( TiXmlElement* pNode )
	{
		ClassInfo* c = Manager::getInstance().getClass( typeid( *this ) );
		if( c )
		{
			LuaCall< void( XUI_Wnd*, const char*, TiXmlElement* ) > op( Lua::Instance(), "save" );
			std::list< const char* > l;
			c->getMembers( l );
			std::list< const char* >::iterator i = l.begin();
			while( i != l.end() )
			{
				try
				{
					OnSavePropertys( *i, pNode );
					op( this, *i, pNode );
				}
				catch( std::runtime_error& err )
				{
					OutputDebugStringA( err.what() );
					return false;
				}
				++i;
			}
		}
		return true;
	}

	bool XUI_Wnd::load_file( TiXmlElement* pNode )
	{
		ClassInfo* c = Manager::getInstance().getClass( typeid( *this ) );
		if( c )
		{
			LuaCall< void( XUI_Wnd*, const char*, TiXmlElement* ) > op( Lua::Instance(), "load" );
			std::list< const char* > l;
			c->getMembers( l );
			std::list< const char* >::iterator i = l.begin();
			while( i != l.end() )
			{
				try
				{
					TiXmlElement* pChild = pNode->FirstChildElement( *i );
					if( pChild )
					{
						op( this, *i, pChild );
						OnLoadPropertys( *i, pChild );
					}
				}
				catch( std::runtime_error& err )
				{
					OutputDebugStringA( err.what() );
					return false;
				}
				++i;
			}
		}
		return true;
	}

	void XUI_Wnd::show_members( int indent )
	{
		ClassInfo* c = Manager::getInstance().getClass( typeid( *this ) );
		if( c )
		{
			LuaCall< const char*( const XUI_Wnd*, const char*, int ) > op( Lua::Instance(), "class_attrib" );
			std::list< const char* > l;
			c->getMembers( l );
			std::list< const char* >::iterator i = l.begin();
			while( i != l.end() )
			{
				try
				{
					_putch( '\n' );
					for( int n = 0; n < indent; ++n ) printf( "  " );
					printf( "|-> %s", *i);
					const char* attrib = op( this, *i, indent );
					if( attrib )
					{
						printf( " = %s", attrib );
					}
				}
				catch( std::runtime_error& err )
				{
					OutputDebugStringA( err.what() );
				}
				++i;
			}
		}
		_putch( '\n' );
	}

	// ����,����
	void XUI_Wnd::OnSavePropertys( const char* name, TiXmlElement* pNode )
	{
	}

	void XUI_Wnd::OnLoadPropertys( const char* name, TiXmlElement* pNode )
	{
		if( strcmp( "window", name ) == 0 )
		{
			OnMoveWindow( GetWindowRect() );
		}
	}
};

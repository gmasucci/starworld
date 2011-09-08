#include "XUI_Header.h"
#include "XUI_LuaBinder.h"
#include "XUI_System.h"
#include "XUI_Commander.h"
//////////////////////////////////////////////////////////////////////////
#include "XUI_Button.h"
#include "XUI_Dialog.h"
#include "XUI_EditBox.h"
#include "XUI_Window.h"
#include "XUI_Factory.h"
//////////////////////////////////////////////////////////////////////////
#include "LuaDebuger.h"
//////////////////////////////////////////////////////////////////////////
#include "SLB\SLB.hpp"
namespace XGC
{
	namespace ui
	{
		XUI_Lua::XUI_Lua(void)
			: m_pLuaDebuger( NULL )
		{
		}

		XUI_Lua::~XUI_Lua(void)
		{
			delete m_pLuaDebuger;
		}

		bool XUI_Lua::SetupDebuger()
		{
			if( m_pLuaDebuger == NULL )
			{
				m_pLuaDebuger = new LuaDebuger();
				m_pLuaDebuger->initialize( getState(), "ui"  );
			}
			return true;
		}

		void XUI_Lua::Initialize()
		{
			SLB::Class< TiXmlElement >( "xml::Element" )
				.constructor< const char* >()
				.set< TiXmlElement, void, const char*, const char* >( "SetAttribute",	&TiXmlElement::SetAttribute )
				.set< TiXmlElement, const char*, const char* >( "Attribute", &TiXmlElement::Attribute )
				.set( "InsertChild",	&TiXmlNode::InsertEndChild )
				.static_inherits< TiXmlNode >()
				;

			SLB::Class< iRect >( "ui::Rect" )
				.constructor< int, int, int, int >()
				.member( "left",	&iRect::left )
				.member( "top",		&iRect::top	)
				.member( "right",	&iRect::right )
				.member( "bottom",	&iRect::bottom )
				.set( "members",	&show_members< iRect > )
				.set( "save",		&save_file< iRect > )
				.set( "load",		&load_file< iRect > )
				;

			SLB::Class< iPoint >( "ui::Point" )
				.constructor< int, int >()
				.member( "x", &iPoint::x )
				.member( "y", &iPoint::y )
				.set( "members",	&show_members< iPoint > )
				.set( "save",		&save_file< iPoint > )
				.set( "load",		&load_file< iPoint > )
				;

			SLB::Class< iSize >( "ui::Size" )
				.constructor< int, int >()
				.member( "cx", &iSize::cx )
				.member( "cy", &iSize::cy )
				.set( "members",	&show_members< iSize > )
				.set( "save",		&save_file< iSize > )
				.set( "load",		&load_file< iSize > )
				;

			SLB::Class< xuiSpriteInfo >( "ui::xuiSpriteInfo" )
				.constructor< _lpcstr, float, float, float, float >()
				.member_readonly( "f",	&xuiSpriteInfo::path )
				.member_readonly( "x",	&xuiSpriteInfo::x )
				.member_readonly( "y",	&xuiSpriteInfo::y )
				.member_readonly( "w",	&xuiSpriteInfo::w )
				.member_readonly( "h",	&xuiSpriteInfo::h )
				.set( "members",&show_members< xuiSpriteInfo > )
				.set( "save",	&xuiSpriteInfo::save_file )
				.set( "load",	&xuiSpriteInfo::load_file )
				;

			SLB::Class< xuiFontInfo >( "ui::xuiFontInfo" )
				.constructor< const char*, int, _uint32, bool, bool, bool >()
				.member_readonly( "name", &xuiFontInfo::m_name )
				.member_readonly( "size", &xuiFontInfo::m_size )
				.member_readonly( "color",&xuiFontInfo::m_color )
				.member_readonly( "bold", &xuiFontInfo::m_bold )
				.member_readonly( "italic",		&xuiFontInfo::m_italic )
				.member_readonly( "antialias",	&xuiFontInfo::m_antialias )
				.set( "members",&show_members< xuiFontInfo > )
				.set( "save",	&xuiFontInfo::save_file )
				.set( "load",	&xuiFontInfo::load_file )
				;

			SLB::Class< XUI_System, SLB::Instance::NoCopyNoDestroy >( "ui::XUI_System" )
				.set( "GetTopWindow",	&XUI_System::GetTopWindow )
				.set( "Load",			&XUI_System::LoadFromFile )
				.set( "Save",			&XUI_System::SaveToFile )
				.set( "GetResPath",		&XUI_System::GetResourcePath )
				;

			SLB::Class< XUI_Wnd, SLB::Instance::NoCopyNoDestroy >( "ui::Wnd" )
				.set( "MoveWindow",		&XUI_Wnd::MoveWindow )
				.set( "OffsetWindow",	&XUI_Wnd::Offset )
				.set( "members",		&XUI_Wnd::show_members )
				.set( "save",			&XUI_Wnd::save_file )
				.set( "load",			&XUI_Wnd::load_file )
				.set( "AddChild",		&XUI_Wnd::AddChild )
				.set( "AddChildAt",		&XUI_Wnd::AddChildAt )
				.set( "RemoveChild",	&XUI_Wnd::RemoveChild )
				.set( "GetParent",		&XUI_Wnd::GetParent )
				.set( "FindChildByName",&XUI_Wnd::FindChildByName )
				.set( "FindChildByID",	&XUI_Wnd::FindChildByID )
				.set( "BringToUp",		&XUI_Wnd::BringToUp )
				.set( "BringToDown",	&XUI_Wnd::BringToDown )
				.set( "BringToFront",	&XUI_Wnd::BringToFront )
				.set( "BringToEnd",		&XUI_Wnd::BringToEnd )
				.member( "name",		&XUI_Wnd::SetName,		&XUI_Wnd::GetName )
				.member( "visible",		&XUI_Wnd::ShowWindow,	&XUI_Wnd::IsVisible )
				.member( "enable",		&XUI_Wnd::EnableWindow,	&XUI_Wnd::IsEnable	)
				.member( "transparent",	&XUI_Wnd::m_bTranslateParent )
				.member( "updatefunc",	&XUI_Wnd::m_strUpdateFunc )
				.member_readonly( "position",	&XUI_Wnd::m_WindowPosition )
				.member_readonly( "size",	&XUI_Wnd::m_WindowSize )
				;

			SLB::Class< XUI_Window, SLB::Instance::NoCopy >( "ui:Window" )
				.static_inherits< XUI_Wnd >()
				.set( "Width",	&XUI_Window::GetWidth )
				.set( "Height", &XUI_Window::GetHeight )
				.member( "x",	&XUI_Window::m_nOffsetX )
				.member( "y",	&XUI_Window::m_nOffsetY )
				;

			SLB::Class< XUI_Button, SLB::Instance::NoCopy >( "ui::Button" )
				.static_inherits< XUI_Wnd >()
				.constructor()
				.set( "SetState",	&XUI_Button::SetState )
				.member( "caption",	&XUI_Button::m_strCaption )
				.member( "skininfo", &XUI_Button::m_ButtonInfo )
				.enumValue( "Normal",		XUI_Button::Normal )
				.enumValue( "MouseOver",	XUI_Button::MouseOver )
				.enumValue( "ButtonDown",	XUI_Button::ButtonDown )
				.enumValue( "Disable",		XUI_Button::Disable )
				;

			SLB::Class< XUI_EditBox, SLB::Instance::NoCopy >( "ui::EditCtrl" )
				.static_inherits< XUI_Wnd >()
				.constructor()
				.member( "text",			&XUI_EditBox::SetText, &XUI_EditBox::GetText )
				.member( "warp",			&XUI_EditBox::m_bWarpText )
				.member( "bordercolor",		&XUI_EditBox::m_dwBorderColor )
				.member( "backgroundcolor",	&XUI_EditBox::m_dwBackgroundColor )
				.member( "singleline",		&XUI_EditBox::m_bSingleLine )
				;

			try
			{
				SLB::Manager::getInstance().set( "CreateSprite",	SLB::FuncCall::create( XUI_CreateSprite ) );
				SLB::Manager::getInstance().set( "DestroySprite",	SLB::FuncCall::create( XUI_DestroySprite ) );

				SLB::Manager::getInstance().set( "CreateFont",		SLB::FuncCall::create( XUI_CreateFont ) );
				SLB::Manager::getInstance().set( "DestroyFont",		SLB::FuncCall::create( XUI_DestroyFont ) );

				SLB::Manager::getInstance().set( "DrawText",		SLB::FuncCall::create( XUI_DrawTextA ) );
				SLB::Manager::getInstance().set( "DrawCharacter",	SLB::FuncCall::create( XUI_DrawCharacterA ) );
				SLB::Manager::getInstance().set( "DrawSprite",		SLB::FuncCall::create( XUI_DrawSprite ) );
				SLB::Manager::getInstance().set( "DrawRect",		SLB::FuncCall::create( XUI_DrawRect ) );

				//////////////////////////////////////////////////////////////////////////
				// lua��غ���
				SLB::Manager::getInstance().set( "SetTimer",		SLB::FuncCall::create( LuaSetTimer ) );
				SLB::Manager::getInstance().set( "KillTimer",		SLB::FuncCall::create( LuaKillTimer ) );
				SLB::Manager::getInstance().set( "CreateUI",		SLB::FuncCall::create( CreateUI ) );

				_tchar path[_MAX_PATH+_MAX_FNAME];
				_tfullpath( path, _T("..\\Resource\\Scripts\\"), _countof( path ) );
				// ���ýű���Ŀ¼
				_string _script( path );
				set( "script", _script );
				set( "gui", &XUI::Instance() );

				_script.append( _T("initialize.lua") );
				doFile( _script.c_str() );
			}
			catch( std::runtime_error& err )
			{
				OutputDebugStringA( err.what() );
				puts( err.what() );
			}
		}

		unsigned int LuaSetTimer( SLB::LuaObject& function, unsigned short repeat, unsigned short timer )
		{
			struct call
			{
				call( SLB::LuaObject& func )
					: function( func )
				{

				}

				~call()
				{

				}

				bool operator()( unsigned int handle, unsigned short& repeat, unsigned int& timer )
				{
					try
					{
						return function( handle, repeat, timer );
					}
					catch( std::runtime_error& error )
					{
						puts( error.what() );
					}

					return false;
				}

				SLB::LuaCall< bool( unsigned int /*handle*/, unsigned short& /*repeat*/, unsigned int& /*timer*/) > function;
			};

			return XUI::Instance().SetTimer( _tfunction( call( function ) ), repeat, timer );
		}

		//////////////////////////////////////////////////////////////////////////
		// ɾ����ʱ��
		//////////////////////////////////////////////////////////////////////////
		void LuaKillTimer( unsigned int handle )
		{
			XUI::Instance().KillTimer( handle );
		}

		XUI_Wnd* CreateUI( _lpcstr lpszLable )
		{
			return XUI_Factory::GetInstance().Creator( lpszLable );
		}
	}
}

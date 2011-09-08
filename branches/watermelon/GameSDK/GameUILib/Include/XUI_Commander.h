#pragma once
#include "singleton.h"

namespace XGC
{
	namespace ui
	{
		class XUI_Wnd;
		class XUI_Commander
		{
			friend struct CreateStatic< XUI_Commander >;
		private:
			XUI_Commander();
			~XUI_Commander();

		public:
			typedef std::vector< _string >	Params;
			//////////////////////////////////////////////////////////////////////////
			// ע������
			//////////////////////////////////////////////////////////////////////////
			typedef bool (XUI_Commander::* Command)( Params& param, _lpctstr helpstring );
			void RegistCommand( _lpctstr lpszCommand, Command func, _lpctstr lpszHelpString );

			//////////////////////////////////////////////////////////////////////////
			// ��������
			//////////////////////////////////////////////////////////////////////////
			bool ProcessCommand( _lpctstr lpszCommand );

			bool Load( _lpcstr lpszFilename );
			bool Save( _lpcstr lpszFilename );
		protected:
			//////////////////////////////////////////////////////////////////////////
			// ������������
			//////////////////////////////////////////////////////////////////////////
			bool cmd_help( Params& param, _lpctstr helpstring );
			bool cmd_save( Params& param, _lpctstr helpstring );
			bool cmd_load( Params& param, _lpctstr helpstring );

			bool cmd_create( Params& param, _lpctstr helpstring );
			bool cmd_delete( Params& param, _lpctstr helpstring );
			bool cmd_attrib( Params& param, _lpctstr helpstring );

			bool cmd_root( Params& param, _lpctstr helpstring );
			bool cmd_child( Params& param, _lpctstr helpstring );
			bool cmd_parent( Params& param, _lpctstr helpstring );
			bool cmd_tree( Params& param, _lpctstr helpstring );
			bool cmd_lua( Params& param, _lpctstr helpstring );

		protected:
			XUI_Wnd* GetElementByPath( _lpctstr lpszPath );

		private:
			struct cmd
			{
				cmd( Command f, _lpctstr s )
					: func( f )
					, helpString( s )
				{}
				Command		func;
				_lpctstr	helpString;
			};
			typedef std::map< _string, cmd >	CCommandMap;
			CCommandMap		m_cmdMap;
			XUI_Wnd*		m_pCurElement;
		};

		inline unsigned int GetLongevity( XUI_Commander* ){ return 22; }
		typedef SingletonHolder< XUI_Commander, CreateStatic, SingletonWithLongevity >	UICommander;
	}
}
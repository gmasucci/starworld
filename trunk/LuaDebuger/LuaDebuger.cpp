#include "StdAfx.h"
#include "LuaDebuger.h"

struct LuaDebuger::Impl
{
	Impl()
		: call_level( 0 )
		, stop_level( 0 )
		, runmode( run )
	{
	}

	~Impl()
	{
	}

	typedef std::set< int >	line_set;
	typedef std::map< std::string, line_set >	break_map;
	typedef std::set< std::string >	path_set;

	// ����ģʽ
	enum run_mode { run, stop, step, stepover, stepout };

	struct variant
	{
		std::string		name;
		unsigned char	type;
		int				stackindex;
	};

	struct stackframe
	{
		int						currentline;
		std::string				filename;
		std::list< variant >	variants;
		std::list< variant >	upvalues;
	};

	typedef std::stack< stackframe >	luastack;

	path_set	paths;
	break_map	breakpoints;

	run_mode	runmode;

	int			call_level;	// ��ǰ��ջ���
	int			stop_level;	// ��ͣʱ�Ķ�ջ���

	luastack	lstack;		// lua ջ
};

struct LuaDebuger::ThreadParam
{
	ThreadParam( LuaDebuger* p, bool (LuaDebuger::* c)( const char* cmd ) )
		: in( INVALID_HANDLE_VALUE )
		, out( INVALID_HANDLE_VALUE )
		, pThis( p )
		, command( c )
	{
		debug_signal = CreateEvent( NULL, TRUE, TRUE, NULL );
	}

	~ThreadParam()
	{
		WaitForSingleObject( debug_signal, INFINITE );
		CloseHandle( debug_signal );
	}

	bool _call( const char* cmd )
	{
		return (pThis->*command)( cmd );
	}
	HANDLE		in, out;
	HANDLE		debug_signal;

private:
	LuaDebuger*	pThis;
	bool (LuaDebuger::* command)( const char* cmd )	;
};

LuaDebuger::LuaDebuger()
: m_pImpl( new Impl )
{

}

LuaDebuger::~LuaDebuger()
{
	delete m_pImpl;
	m_pImpl = NULL;
}

void LuaDebuger::bp( const char* name, int line )
{
	if( name != NULL && line >= 0 )
	{
		m_pImpl->breakpoints[name].insert( line );
	}
}

bool LuaDebuger::judgeBreak( const char* name, int line )
{
	Impl::break_map::const_iterator citer = m_pImpl->breakpoints.find( name );
	if( citer != m_pImpl->breakpoints.end() )
	{
		const Impl::line_set &lineset = citer->second;
		Impl::line_set::const_iterator cline = lineset.find( line );
		if( cline != lineset.end() )
		{
			return true;
		}
	}
	return false;
}

bool LuaDebuger::waitSignal( lua_State *L )
{
	if( m_thread_param == NULL ) 
	{
		return false;
	}

	DWORD dwRet = WaitForSingleObject( m_thread_param->debug_signal, INFINITE );
	if( dwRet != WAIT_OBJECT_0 )
	{
		luaL_error( L, "debug signal error." );
		return false;
	}
	return true;
}

static void line_hook( LuaDebuger* pDebuger, lua_State *L, lua_Debug *ar )
{
	switch( pDebuger->m_pImpl->runmode )
	{
	case LuaDebuger::Impl::stop:
		pDebuger->waitSignal(L);
		break;
	case LuaDebuger::Impl::step:
		if( pDebuger->m_pImpl->stop_level >= pDebuger->m_pImpl->call_level )
		{
			pDebuger->waitSignal(L);
		}
		break;
	case LuaDebuger::Impl::stepout:
		if( pDebuger->m_pImpl->stop_level > pDebuger->m_pImpl->call_level )
		{
			pDebuger->waitSignal(L);
		}
		break;
	case LuaDebuger::Impl::run:
		if( ar->source[0] == '@' && pDebuger->judgeBreak( ar->source+1, ar->currentline ) )
		{
			lua_getinfo( L, "nu", ar );
			pDebuger->waitSignal(L);
		}
		break;
	}
}

static void call_hook( LuaDebuger* pDebuger, lua_State *L, lua_Debug *ar )
{
	++ pDebuger->m_pImpl->call_level;
}

static void ret_hook( LuaDebuger* pDebuger, lua_State *L, lua_Debug *ar )
{
	-- pDebuger->m_pImpl->call_level;
}

static void count_hook( LuaDebuger* pDebuger, lua_State *L, lua_Debug *ar )
{

}

static void Debug(lua_State *L, lua_Debug *ar)
{
	lua_getglobal( L, "__debuger" );
	LuaDebuger* pDebuger = (LuaDebuger*)lua_touserdata( L, -1 );

	switch( ar->event )
	{
	case LUA_HOOKCOUNT:
		count_hook( pDebuger, L, ar);
		break;

	case LUA_HOOKCALL:
		call_hook( pDebuger, L, ar);
		break;

	case LUA_HOOKRET:
	case LUA_HOOKTAILRET:	//verify
		ret_hook( pDebuger, L, ar);
		break;

	case LUA_HOOKLINE:
		lua_getinfo( L, "Sl", ar );
		line_hook( pDebuger, L, ar );
		break;
	}

	//char szFilename[_MAX_FNAME];
	//char szExt[_MAX_EXT];
	//std::stringstream filename;
	//lua_getstack(L, 0, &debug );
	//if( lua_getinfo(L, "Sln", &debug) )
	//{
	//	_splitpath_s( debug.source, NULL, 0, NULL, 0, szFilename, _countof(szFilename), szExt, _countof(szExt) );
	//	{
	//		filename << szFilename << szExt;
	//		pDebuger->is_break( filename.str().c_str(), debug.currentline );
	//	}
	//}
}

unsigned int __stdcall LuaDebuger::guard( void *param )
{
	LuaDebuger::ThreadParam* p = ( LuaDebuger::ThreadParam* )param;

	TCHAR	buffer[BUFSIZE];
	DWORD	dwRead, dwWrite;
	HANDLE	hPipe;
	BOOL	fConnected; 
	LPTSTR	lpszPipename = TEXT("\\\\.\\pipe\\lua_debuger"); 

	// The main loop creates an instance of the named pipe and 
	// then waits for a client to connect to it. When the client 
	// connects, a thread is created to handle communications 
	// with that client, and the loop is repeated. 

	for (;;) 
	{ 
		hPipe = CreateNamedPipe( 
			lpszPipename,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			1,						  // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
			NULL);                    // default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE) 
		{
			printf("CreatePipe failed"); 
			return 0;
		}

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

		fConnected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 

		while(fConnected)
		{ 
			BOOL ret = ReadFile( hPipe, buffer, sizeof(buffer), &dwRead, NULL);
			if( ret )
			{
				p->_call( (const char*)buffer );
			}
		}
	}

	return 0;
}

bool LuaDebuger::initialize( lua_State* L )
{
	m_thread_param = new ThreadParam( this, &LuaDebuger::command );
	
	lua_sethook( L, Debug, LUA_MASKCALL|LUA_MASKLINE|LUA_MASKRET|LUA_MASKCOUNT, 1 );
	lua_pushlightuserdata( L, this );
	lua_setglobal( L, "__debuger" );

	m_thread_param->debug_signal = (HANDLE)_beginthreadex( NULL, 0, guard, m_thread_param, 0, NULL );
	return true;
}

bool LuaDebuger::command( const char* cmd )
{
	return true;
}
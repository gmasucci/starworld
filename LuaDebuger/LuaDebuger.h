#pragma once

#include "LuaDebuger.h"
struct lua_State;
class LuaDebuger
{
public:
	LuaDebuger();
	~LuaDebuger();

	bool initialize( lua_State* L );
	void terminal();

	bool command( const char* cmd );

	void bp( const char* name, int line );
	void step();
	void stepover();
	void stepout();
	void pause();
	void run();
	void stop();

protected:
	friend void Debug( lua_State *L, lua_Debug* ar );
	friend void line_hook( LuaDebuger *pDebuger, lua_State *L, lua_Debug *ar );
	friend void call_hook( LuaDebuger *pDebuger, lua_State *L, lua_Debug *ar );
	friend void ret_hook( LuaDebuger *pDebuger, lua_State *L, lua_Debug *ar );
	friend void count_hook( LuaDebuger *pDebuger, lua_State *L, lua_Debug *ar );

	bool judgeBreak( const char* name, int line );
	bool waitSignal( lua_State *L );

	static unsigned int __stdcall guard( void *param );
private:
	struct Impl;
	struct ThreadParam;
	Impl*			m_pImpl;
	ThreadParam*	m_thread_param;
};

// LuaDebugConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LuaDebugCommander.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LuaDebugCommander commander;

	LPTSTR	lpszPipename = TEXT("\\\\.\\pipe\\lua_debuger"); 

	commander.initialize( lpszPipename );
	return 0;
}


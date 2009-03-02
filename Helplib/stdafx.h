// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#define _WIN32_WINNT 0x0500
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <Windows.h>
#include <MMSystem.h>
#include <WinSock2.h>
#include <atlconv.h>
// crt
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <process.h>

// stl
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <list>
#include <algorithm>

// common head file
#include "macro_define.h"
#include "logger.h"
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "atls.lib" )

#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

// GameWorld.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GameWorld.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       uint32  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// ���ǵ���������һ��ʾ��
GAMEWORLD_API int nGameWorld=0;

// ���ǵ���������һ��ʾ����
GAMEWORLD_API int fnGameWorld(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� GameWorld.h
CGameWorld::CGameWorld()
{ 
	return; 
}

// MessageCreator.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "bufstream.h"
#include "AnalyseFile.h"

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	root r;
	for( int i = 1; i < argc; ++i )
	{
		analysefile( &r, argv[i] );
	}
	return 0;
}
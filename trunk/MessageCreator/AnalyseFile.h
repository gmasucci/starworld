#pragma once
#include <list>
#include <map>
using namespace std;
struct param
{
	string	tname;
	string	pname;
	bool	_array;	// �Ƿ�����
	bool	_point; // �Ƿ�ָ��
};

struct node
{
	list< param >	params;
	list< node >	sub;

	int container_num; // ��������
};

struct message
{
	node sub;
	string messagename;
	string stype;
	string scode;
};

struct root
{
	char	*buf;
	size_t	size;
	node	*pnode;
	map< string, string >	definemap;	// Ԥ�����
	map< string, string >	typedefmap;	// ���Ͷ����
	map< string, int >		mcode;
	list< node >	snode;	// �ṹ����
	list< message >	mnode;	// ��Ϣ����
};
struct command;

//--------------------------------------------------------//
//	created:	14:12:2009   16:53
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����ַ�
//--------------------------------------------------------//
char* trim( char *buf, size_t size, char *trimstring );

//--------------------------------------------------------//
//	created:	14:12:2009   14:51
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����ļ�
//--------------------------------------------------------//
int analysefile( root *proot, _lpctstr filename );

//--------------------------------------------------------//
//	created:	14:12:2009   15:01
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����б��еĵ�һ���ַ���
//--------------------------------------------------------//
int findkeywork( char **buf, size_t *size, command commands[], int count );

//--------------------------------------------------------//
//	created:	14:12:2009   18:14
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������
//--------------------------------------------------------//
size_t makeparam( root *proot, char *buf, size_t size );

//--------------------------------------------------------//
//	created:	14:12:2009   14:51
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������ṹ
//--------------------------------------------------------//
size_t maketree( root *proot, char *buf, size_t size );

//--------------------------------------------------------//
//	created:	14:12:2009   17:23
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	������Ϣ
//--------------------------------------------------------//
size_t makemessage( root *proot, char *buf, size_t size );
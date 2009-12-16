#pragma once
#include <list>
#include <stack>
using namespace std;
struct param
{
	string	tline;
	string	comment;
	string	tname;
	string	pname;
	bool	_array;	// �Ƿ�����
	bool	_point; // �Ƿ�ָ��
	bool	_container; // �Ƿ�����
};

struct node
{
	string			name;
	list< param* >	params;
	list< node* >	sub;
};

struct message
{
	node sub;
	string stype;
	string scode;
};

struct root
{
	size_t	size;
	char	*buf;
	stack< node * >			pnode;
	map< string, string >	definemap;	// Ԥ�����
	map< string, int >		mcode;
	list< node* >			snode;	// �ṹ����
	list< message* >		mnode;	// ��Ϣ����
};
struct command;

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
size_t makeparam( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   18:14
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������δ�
//--------------------------------------------------------//
size_t makemodifier( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   18:14
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������
//--------------------------------------------------------//
size_t makebasic( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   18:14
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������
//--------------------------------------------------------//
size_t makecontainer( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   14:51
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�������ṹ
//--------------------------------------------------------//
size_t maketree( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   17:23
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	������Ϣ
//--------------------------------------------------------//
size_t makemessage( root *proot, char *buf, size_t size, void* pdata );
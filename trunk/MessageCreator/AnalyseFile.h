#pragma once
#include <list>
#include <stack>
#include <fstream>
#include <strstream>
using namespace std;
struct param
{
	string	tline;
	string	comment;
	string	tname;
	string	pname;
	string	pvalue;
	bool	_immediately;	// �Ƿ�������
	bool	_array;			// �Ƿ�����
	bool	_point;			// �Ƿ�ָ��
	bool	_basic;			// �Ƿ��������
	bool	_container;		// �Ƿ�����
};

struct node
{
	string			name;
	string			spacename;
	list< param* >	params;
	list< node* >	sub;
};

struct message
{
	node sub;
	string stype;
	string scode;
	string filename;
};

struct root
{
	size_t	size;
	char	*buf;
	string	filename;
	string	bindir;		// ���ļ����Ŀ¼
	string	outdir;		// �����ļ����Ŀ¼
	string	libdir;		// ���ļ����·��
	fstream	hfile;		// .h �ļ�
	fstream	cfile;		// .cpp �ļ�
	map< string, list< string > >	mcode;		// ��Ϣ�ṹ
	stack< node * >		pnode;
	list< node* >		snode;		// �ṹ����
	list< message* >	mnode;		// ��Ϣ����
	list< string >		defines;	// Ԥ�����
	list< string >		files;
	struct
	{
		bool	_debug;
		bool	_runtime;
		bool	_library;
		string	version;		// �汾
		string	runtime;		// ���п�
		string	linktar;		// ����Ŀ�� dll or lib
	}opt;
};
struct command;

//--------------------------------------------------------//
//	created:	18:12:2009   16:37
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����﷨��
//--------------------------------------------------------//
void destroyall( root *proot );

//--------------------------------------------------------//
//	created:	18:12:2009   16:37
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	������Ϣ
//--------------------------------------------------------//
void destroymessage( message *pmessage );

//--------------------------------------------------------//
//	created:	18:12:2009   16:37
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	���ٽڵ�
//--------------------------------------------------------//
void destroynode( node *pnode );

//--------------------------------------------------------//
//	created:	18:12:2009   16:37
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	���ٲ���
//--------------------------------------------------------//
void destroyparam( param *pparam );

//--------------------------------------------------------//
//	created:	14:12:2009   14:51
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����ļ�
//--------------------------------------------------------//
int analysefile( root *proot, _lpcstr filename );

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
//	purpose:	������Ϣ����
//--------------------------------------------------------//
size_t makedefine( root *proot, char *buf, size_t size, void* pdata );

//--------------------------------------------------------//
//	created:	14:12:2009   18:14
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	����ṹ����
//--------------------------------------------------------//
size_t makenode( root *proot, char *buf, size_t size, void* pdata );

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

//--------------------------------------------------------//
//	created:	17:12:2009   14:28
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	дһ����Ϣ
//--------------------------------------------------------//
void writemessage( root *proot, message *pmessage );

//--------------------------------------------------------//
//	created:	17:12:2009   14:29
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	дһ���ṹ
//--------------------------------------------------------//
void writenode_def( root *proot, node *pnode, string pix );
void writenode_imp( root *proot, node *pnode, string pix );

//--------------------------------------------------------//
//	created:	18:12:2009   15:24
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	����ͷ�ļ�
//--------------------------------------------------------//
void writeheader( root *proot );

//--------------------------------------------------------//
//	created:	17:12:2009   14:27
//	filename: 	AnalyseFile
//	author:		Albert.xu
//
//	purpose:	�����ɵ���Ϣ��ת����C++�ļ���
//--------------------------------------------------------//
void writefile( root *proot );
#pragma once
#ifndef _DB_H
#define _DB_H
#include <queue>
#include "IDB.h"
#include "lookaside.h"
#include "critical_lock.h"
#include "macro_define.h"

#ifdef USE_MYSQL
	#pragma comment( lib, "libmySQL.lib" )
#endif // USE_MYSQL

#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS
#pragma warning( disable :	4251 ) 

class TOOLLIB_API CDBRequest;
class TOOLLIB_API CConnection;

//##ModelId=46722CE2003E
class TOOLLIB_API CConnMng
{
public:
	//##ModelId=46722CE2004E
	CConnMng();
	//##ModelId=46722CE2004F
	~CConnMng();

	//////////////////////////////////////////////////////////////////////////
	// ��ʼ���ݿ����
	// return �����Ƿ�ɹ�����
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE20050
	bool DBStart();

	//////////////////////////////////////////////////////////////////////////
	// ֹͣ���ݿ����
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE20051
	void DBStop();

	//////////////////////////////////////////////////////////////////////////
	// ע��һ������
	// lpszConnString	:	�����ַ���
	// nHartTime		:	����ʱ��
	// nExecuteTime		:	�����ݿ�����ʱ�ĵȴ�ʱ��
	// return			:	�Ƿ�ɹ��������ݿ����ӱ�
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE2005D
	bool RegistConnection( LPCTSTR lpszConnString, int nHartTime = 1000, int nExecuteTime = 100 );

	//////////////////////////////////////////////////////////////////////////
	// ɾ��һ������
	// ������Ϊ���Ӷ�ʹ����ͬ�ı�
	// return	:	�ж��ٸ����ӱ����
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE20061
	int UnRegistConnection( LPCTSTR lpszConnString );

	//////////////////////////////////////////////////////////////////////////
	// Ͷ��һ�����ݿ�����
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE20063
	void PostRequest( IDBRequest* pRequest );

private:
	//##ModelId=46722CE2006D
	static unsigned int __stdcall DBThreadLoop( void* lpParam );

	//##ModelId=46722CE2009C
	typedef struct _THREADPARAM_
	{
		_string			strConnString;
		volatile LONG	bWork;
		IConnection*	pConn;
		int				nHartTime;
		int				nExecuteTime;
		HANDLE			hHandle;
	}*PTHREADPARAM, THREADPARAM;

	typedef std::vector< PTHREADPARAM >	CThreadParamVec;
	//##ModelId=46722CE20070
	CRITICAL_SECTION	m_lock;
	//##ModelId=46722CE2008D
	CThreadParamVec		m_ParamVec;
	//##ModelId=46722CE20091
	bool				m_bStart;
};

//##ModelId=46722CE200BB
class TOOLLIB_API CConnection :	public IConnection
{
friend class CRecordset;
public:
	//##ModelId=46722CE200BD
	CConnection(void);
	//##ModelId=46722CE200BF
	~CConnection(void);

	// �����ݿ�
	// szConn	:	���Ӵ�
	// return	:	true ���ӳɹ�	false ����ʧ��
	// remark	:	һ�����������Ӵ�ʾ��
	// -ip 127.0.0.1 -port 3369 -user user -pwd password -database files
	//##ModelId=46722CE200CB
	virtual bool open( LPCSTR szConn );

	// Ͷ�����ݿ������
	//##ModelId=46722CE200CE
	virtual bool request( IDBRequest* pRequest );

	//##ModelId=46722CE200D1
	virtual IRecordset* execute( const char* szSql );
	//##ModelId=46722CE200D4
	virtual bool execute_request();

private:
	struct Impl;
	//##ModelId=46722CE200DB
	Impl* m_Impl;
	//##ModelId=46722CE20104
	typedef std::queue< IDBRequest* >	CRequestQueue;

	//##ModelId=46722CE200E0
	_string		m_strIP;
	//##ModelId=46722CE200E5
	_string		m_strDatabase;
	//##ModelId=46722CE200EA
	_string		m_strUser;
	//##ModelId=46722CE200EF
	_string		m_strPwd;
	//##ModelId=46722CE200F3
	int				m_nPort;
	//##ModelId=46722CE200F5
	CRequestQueue	m_RequestQueue;

	//##ModelId=46722CE200FB
	CCritical		m_Lock;
	//##ModelId=46722CE20100
	CCritical		m_ExecuteLock;
};

//##ModelId=46722CE20138
class TOOLLIB_API CRecordset	:	public IRecordset
{
friend class CConnection;
public:
	//##ModelId=46722CE20149
	CRecordset(void);
	//##ModelId=46722CE2014B
	~CRecordset(void);

	//##ModelId=46722CE2014D
	virtual bool	open( const IConnection* pConn, const char* sql );
	//##ModelId=46722CE20151
	virtual bool	add_new();
	//##ModelId=46722CE20153
	virtual bool	edit();
	//##ModelId=46722CE20155
	virtual bool	update();
	//##ModelId=46722CE20158
	virtual long	get_record_count();

	//##ModelId=46722CE2015A
	virtual bool	next_row();
	//##ModelId=46722CE2015C
	virtual bool	prve_row();

	//##ModelId=46722CE2015E
	virtual long		get_field_count();
	//##ModelId=46722CE20160
	virtual const char* get_field_name( int nIndex );

	//##ModelId=46722CE20163
	virtual bool		set_field_value( const char* field_name, const char* field_value );
	//##ModelId=46722CE20167
	virtual bool		set_field_value( int nIndex, const char* field_value );

	//##ModelId=46722CE2016B
	virtual const char*	get_field_value( const char* field_name );
	//##ModelId=46722CE2016E
	virtual const char* get_field_value( int nIndex );
	//##ModelId=46722CE20171
	virtual void		release();

private:
	struct Impl;
	//##ModelId=46722CE20174
	Impl* m_Impl;
};

//##ModelId=46722CE201A5
class TOOLLIB_API CDBRequest	:	public IDBRequest
{
public:
	//##ModelId=46722CE201B5
	CDBRequest();
	//##ModelId=46722CE201B6
	virtual ~CDBRequest();

	//////////////////////////////////////////////////////////////////////////
	// ������������
	// nClientID	��	��������Ŀͻ���ID
	// szSQL		��	�����SQL��䣬��0��β
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE201B8
	virtual void SetRequest( int nUserID, int nTransmitID, const char* szSQL );

	//////////////////////////////////////////////////////////////////////////
	// �õ�������
	// return		��	�����
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE201BD
	virtual IRecordset* GetRecordset();

	//////////////////////////////////////////////////////////////////////////
	// ����ִ��ʱ����
	// pConnection	��	���ݿ����Ӷ���ͨ���ö������execute
	// return		��	�Ƿ���ȷִ��
	// remark
	//		����ֵΪtrueʱ���������ִ�У�ֱ������������ݸ����÷���
	//		����ֵΪfalseʱ�����������������
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE201BF
	virtual bool OnExecute( IConnection* pConnection );

	//////////////////////////////////////////////////////////////////////////
	// ѡ���ĸ����ݿ����ӽ��в���
	// nConnectionCount	:	���ݿ����
	// return			:	ʹ�õ����ݿ�����,����Ϊ��Чֵ
	//////////////////////////////////////////////////////////////////////////
	//##ModelId=46722CE201C2
	virtual int HashConnection( int nConnectionCount );

protected:
	//##ModelId=46722CE201C5
	int			m_nUserID;
	//##ModelId=46722CE201C6
	int			m_nTransmitID;
	//##ModelId=46722CE201C7
	bool		m_bFinish;
	//##ModelId=46722CE201C9
	_string		 m_strSql;

private:
	//##ModelId=46722CE201CE
	IRecordset* m_pRS;
};
#endif // _DB_H

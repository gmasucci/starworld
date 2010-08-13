#pragma once
#include "TransQueue.h"

class CDatabaseService;
typedef message_dispatcher
< 
	CDatabaseService, 
	size_t (CDatabaseService::*)( db::connection conn, transaction *header ), 
	short 
> 
DbDispatcherBase;

class CDatabaseService	:	public DbDispatcherBase
{
	IMPLEMENT_DISPATCHER_TABLE( database );
private:
	uintptr_t		m_thread_h;
	bool			m_work_b;
	_astring		m_connstring;

	// �������
	CTransQueue		m_transqueue;

public:
	CDatabaseService(void);
	~CDatabaseService(void);

	//--------------------------------------------------------//
	//	created:	29:12:2009   16:48
	//	filename: 	DatabaseService
	//	author:		Albert.xu
	//
	//	purpose:	�������ݿ����
	//--------------------------------------------------------//
	bool start( _lpcstr connstring );
	void stop();

protected:
	// ���ݿ�����߳�
	static unsigned int __stdcall svc( _lpvoid pParam );

	//--------------------------------------------------------//
	//	created:	29:12:2009   16:49
	//	filename: 	DatabaseService
	//	author:		Albert.xu
	//
	//	purpose:	�������ݿ�����
	//--------------------------------------------------------//
	size_t process( db::connection conn, transaction *header );

	//--------------------------------------------------------//
	//	created:	3:12:2009   13:11
	//	filename: 	DispatchDatabase
	//	author:		Albert.xu
	//
	//	purpose:	ע��һ���˺�
	//--------------------------------------------------------//
	size_t userRegist( db::connection conn, transaction *header );

	//--------------------------------------------------------//
	//	created:	3:12:2009   13:11
	//	filename: 	DispatchDatabase
	//	author:		Albert.xu
	//
	//	purpose:	�û���½
	//--------------------------------------------------------//
	size_t userLogon( db::connection conn, transaction *header );
};

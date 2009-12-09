#pragma once
struct dbMsgHeader
{
	size_t	size;	// ��Ϣ����
	size_t	uid;	// �û�id
	size_t	mid;	// ��Ϣ��
};

class CDispatcherDatabase;
typedef message_dispatcher< CDispatcherDatabase, size_t (CDispatcherDatabase::*)( dbMsgHeader &header ), short > DispatcherBase;

class CDispatcherDatabase	:	public DispatcherBase
{
	IMPLEMENT_DISPATCHER_TABLE( database );
public:
	CDispatcherDatabase(void);
	~CDispatcherDatabase(void);

	size_t Process( dbMsgHeader &header );
protected:
	//--------------------------------------------------------//
	//	created:	3:12:2009   13:11
	//	filename: 	DispatchDatabase
	//	author:		Albert.xu
	//
	//	purpose:	ע��һ���˺�
	//--------------------------------------------------------//
	size_t registerUser( dbMsgHeader &header );
};

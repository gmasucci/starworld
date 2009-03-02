#pragma once
#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

class TOOLLIB_API IRecordset;
class TOOLLIB_API IDBRequest;

class TOOLLIB_API IConnection
{
public:
	virtual ~IConnection(){};
	virtual bool		open( LPCSTR szConn ) = 0;
	virtual bool		request( IDBRequest* pRequest ) = 0;

	virtual IRecordset* execute( const char* szSql ) = 0;
	virtual bool		execute_request() = 0;
};

class TOOLLIB_API IRecordset
{
public:
	~IRecordset(void){};

	virtual bool		open( const IConnection* pConn, const char* sql ) = 0;
	virtual bool		add_new() = 0;
	virtual bool		edit() = 0;
	virtual bool		update() = 0;
	virtual long		get_record_count() = 0;
	virtual bool		next_row() = 0;
	virtual bool		prve_row() = 0;

	virtual long		get_field_count() = 0;
	virtual const char* get_field_name( int nIndex ) = 0;
	
	virtual bool		set_field_value( const char* field_name, const char* field_value ) = 0;
	virtual bool		set_field_value( int nIndex, const char* field_value ) = 0;

	virtual const char*	get_field_value( const char* field_name ) = 0;
	virtual const char* get_field_value( int nIndex ) = 0;
	virtual void		release() = 0;
};

class TOOLLIB_API IDBRequest
{
public:
	virtual ~IDBRequest(){}

	//////////////////////////////////////////////////////////////////////////
	// ������������
	// nClientID	��	��������Ŀͻ���ID
	// szSQL		��	�����SQL��䣬��0��β
	//////////////////////////////////////////////////////////////////////////
	virtual void SetRequest( int nUserID, int nTransmitID, const char* szSQL ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// �õ�������
	// return		��	�����
	//////////////////////////////////////////////////////////////////////////
	virtual IRecordset* GetRecordset() = 0;

	//////////////////////////////////////////////////////////////////////////
	// ����ִ��ʱ����
	// pConnection	��	���ݿ����Ӷ���ͨ���ö������execute
	// return		��	�Ƿ���ȷִ��
	// remark
	//		����ֵΪtrueʱ���������ִ�У�ֱ������������ݸ����÷���
	//		����ֵΪfalseʱ�����������������
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnExecute( IConnection* pConnection )	= 0;

	//////////////////////////////////////////////////////////////////////////
	// ѡ���ĸ����ݿ����ӽ��в���
	// nConnectionCount	:	���ݿ����
	// return			:	ʹ�õ����ݿ�����,����Ϊ��Чֵ
	//////////////////////////////////////////////////////////////////////////
	virtual int HashConnection( int nConnectionCount ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// �н������ʱ����
	// pRecordset	��	���������
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnResult()		= 0;

	//////////////////////////////////////////////////////////////////////////
	// �ͷ��ڴ�
	//////////////////////////////////////////////////////////////////////////
	virtual void Release() = 0;
};
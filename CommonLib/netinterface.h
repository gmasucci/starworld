#pragma once
#ifndef _INET_INTERFACE
#define _INET_INTERFACE
#include "defines.h"
#include <queue>
#define SYSTEM_MESSAGE_TYPE	0
#define EVENT_ACCEPT	0
#define EVENT_CLOSE		1
#define EVENT_CONNECT	2
#define EVENT_ERROR		3

// param [in]: in - [1]{ self_handle }
// return �ɹ� �����µ���ID -1 û�п��õ�����
#define Operator_NewGroup		0	

// param [in]: in - GroupParam[2] = { _uint32 handle; _uint32 group; }
// reutrn 0 �ɹ� -1 ָ�������Ѳ����� -2 �������ѱ���� -3 ��Ӿ������ʧ��
#define Operator_EnterGroup		1	

// param [in]: in - GroupParam[2] = { _uint32 handle; _uint32 group; }
// return 0 �ɹ� -1 ָ�������Ѳ����� -2 ������������
#define Operator_LeaveGroup		2	

// param [in]: in - GroupIndex[1] = { _uint32 group }
// return 0 �ɹ� -1 ָ�������Ѳ�����
#define Operator_RemoveGroup	3	

// param [in,out]: int - handle, out - userdata
#define Operator_GetUserdata	4	

typedef bool (*fnRecvMessage)( void* data, size_t size );
struct __declspec(novtable, dllexport) IReciver
{
	virtual
		~IReciver(){}

	virtual
		int
		recv_message( const char *data, size_t size ) = 0;
};

struct __declspec(novtable, dllexport) INet
{
	//////////////////////////////////////////////////////////////////////////
	// �������ݰ�
	//////////////////////////////////////////////////////////////////////////
	virtual
		int		
		send( char* data, size_t size ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ������Ϣ������
	// fnRecv
	//////////////////////////////////////////////////////////////////////////
	virtual
		bool
		recv( IReciver& reciver ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ���ӷ�����
	//////////////////////////////////////////////////////////////////////////
	virtual
		bool	
		connect( const char* host, unsigned short port ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// �Ͽ�����������
	//////////////////////////////////////////////////////////////////////////
	virtual
		void	
		disconnect() = 0;
};

struct __declspec(novtable, dllexport) INetPacket
{
	virtual _lpcstr		data()const = 0;
	virtual size_t		size()const = 0;
	virtual size_t		capacity()const = 0;
	virtual size_t		handle()const = 0;
	virtual long_ptr	userdata()const = 0;
	virtual void		release() = 0;
};

struct __declspec(novtable, dllexport) IMessageQueue
{
	virtual INetPacket* PickMessage() = 0;
	virtual INetPacket* PopMessage() = 0;
	virtual void PushMessage( INetPacket* ) = 0;
	virtual void Release() = 0;
};

typedef bool (*pfnInitNetwork)();
typedef void (*pfnFiniNetwork)();
typedef bool (*pfnStartServer)( _lpcstr address, _uint16 port, IMessageQueue** msg_queue );
typedef void (*pfnCloseServer)();
typedef bool (*pfnConnectServer)( _lpcstr address, _uint16 port, IMessageQueue** msg_queue, _uint32 time_out );
typedef void (*pfnSendPacket)( size_t handle, _lpcstr data, size_t size );
typedef void (*pfnSendPackets)( size_t *handle, _uint32 count, _lpcstr data, size_t size );
typedef void (*pfnSendToGroup)( _uint32 group, _lpcstr data, size_t size, bool toself );
typedef void (*pfnCloseLink)( size_t handle );
typedef void (*pfnBindUserdata)( size_t handle, long_ptr user );
typedef long (*pfnExecuteState)( _uint32 operate_code, long_ptr param );

#ifdef NETBASE_EXPORTS
#define NETBASE_API __declspec(dllexport)
#else
#define NETBASE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

NETBASE_API extern pfnInitNetwork	InitNetwork;
NETBASE_API extern pfnFiniNetwork	FiniNetwork;
NETBASE_API extern pfnStartServer	StartServer;
NETBASE_API extern pfnCloseServer	CloseServer;
NETBASE_API extern pfnConnectServer	ConnectServer;
NETBASE_API extern pfnSendPacket	SendPacket;
NETBASE_API extern pfnSendPackets	SendPackets;
NETBASE_API extern pfnSendToGroup	SendToGroup;
NETBASE_API extern pfnCloseLink		CloseLink;
NETBASE_API extern pfnBindUserdata	BindUserdata;
NETBASE_API extern pfnExecuteState	ExecuteState; // ���� 0 ��ʾ�ɹ�, < 0 ��ʾ�������� > 0 ��ʾ��������ִ��,������������.
#endif //_INET_INTERFACE
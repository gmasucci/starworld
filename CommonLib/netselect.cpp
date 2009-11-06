#include "stdafx.h"
#include "NetSelect.h"
#include <winsock.h>

CNetSelect::CNetSelect()
: m_socket( INVALID_SOCKET )
{
}

CNetSelect::~CNetSelect()
{
}

VOID CNetSelect::SendErrorCodeEx( int nErrorCode )
{
	m_nErrorCode = nErrorCode;
}

int	CNetSelect::InitSocket()
{
	WSADATA wsd;
	// ����winsock��
	int nResult = WSAStartup(MAKEWORD(1,1), &wsd);
	if (0 != nResult)		return FALSE;

	// ���汾
	if ( LOBYTE( wsd.wVersion ) != 1 || HIBYTE( wsd.wVersion ) != 1 )
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

HRESULT	CNetSelect::Connect( const char *address, unsigned short port )	// ��������
{
	SOCKADDR_IN		ServerAddr;

	ServerAddr.sin_family		= AF_INET;
	ServerAddr.sin_addr.s_addr	= inet_addr( address );
	ServerAddr.sin_port			= htons( port );

	m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	int overtime = 1000;
	// SO_RCVTIMEO �� SO_SNDTIMEO �п�������Ǳ�ڵ����ݶ�ʧ�������.
	setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&overtime, sizeof( overtime ) );
	setsockopt( m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&overtime, sizeof( overtime ) );
	int hResult = connect( m_socket, ( SOCKADDR *)&ServerAddr, sizeof( ServerAddr ) );
	if( hResult == SOCKET_ERROR )
	{
		int nErrorCode = WSAGetLastError();
		SendErrorCodeEx( nErrorCode );
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

BOOL CNetSelect::Disconnect()
{
	//��ȫ�ر��׽���
	int hResult = shutdown( m_socket, 0 );
	if( hResult == SOCKET_ERROR )
	{
		SendErrorCodeEx( WSAGetLastError() );
	}
	hResult = closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	if( hResult == SOCKET_ERROR )
	{
		SendErrorCodeEx( WSAGetLastError() );
		return FALSE;
	}
	return TRUE;
}

HRESULT CNetSelect::Send( const char* pszBuffer, int size, int& nResult )
{
	// ������Ϣͷ
	nResult = send( m_socket, pszBuffer, size, 0 );
	if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		SendErrorCodeEx( nResult );
		int nRet = SUCCESS;
		if( nResult == WSAETIMEDOUT )
		{
			nRet = RETRY;
		}
		if( nResult == WSAECONNABORTED ||
			nResult == WSAECONNRESET ||
			nResult == WSAENOTSOCK ||
			nResult == WSAENOTCONN )	// ��ʱ����Э�����
		{
			// ����ֱ�ӷ��أ��ȴ���һ�η��͵��á�
			//Disconnect();
			nRet = DISCONNECT;
		}
		return nRet;
	}
	return SUCCESS;
}

HRESULT CNetSelect::Recv( char* pszBuffer, int size, int& nResult )
{
	fd_set fdread;
	timeval tv;
	FD_ZERO(&fdread);//��ʼ��fd_set

	tv.tv_sec = 0;//�������Ǵ�����select�ȴ�200mil�󷵻أ����ⱻ������Ҳ�������Ϸ���
	tv.tv_usec = 10;

	FD_SET(m_socket, &fdread);//�����׽��־������Ӧ��fd_set
	nResult = select(0, &fdread, NULL, NULL, &tv);
	if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		return nResult == WSAECONNRESET || nResult == WSAENOTSOCK?DISCONNECT:SOCKETERROR;
	}
	else if( FD_ISSET( m_socket, &fdread ) )
	{
		nResult = recv( m_socket, pszBuffer, size, 0 );
		if( nResult == SOCKET_ERROR )
		{
			nResult = WSAGetLastError();
			//Disconnect();
			return (nResult == WSAENOTSOCK||nResult== WSAECONNRESET)?DISCONNECT:SOCKETERROR;
		}
		else if( nResult == 0 )
		{
			//Disconnect();
			return DISCONNECT;
		}
		return SUCCESS;
	}
	return RETRY;
}
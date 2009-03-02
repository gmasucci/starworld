#pragma once
#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

#include <WinSock2.h>
class TOOLLIB_API CNetEvent;
class TOOLLIB_API ISocketBase
{
	friend class CNetEvent;
public:
	operator SOCKET()const{ return s; }
	SOCKET operator=( SOCKET sock ){ s = sock; return s; }
private:
	virtual void OnAccept()	= 0;
	virtual void OnRead()	= 0;
	virtual void OnWrite()	= 0;
	virtual void OnClose()	= 0;
	virtual void OnConnect()= 0;

	virtual void Release()	= 0;
private:
	SOCKET	s;
	LONG	hHandle;
};

/*
	����EventSelect ģ����һ�ֱ������ݽ��յ�ģ�ͣ�������Ҫһ��
	�߳���������Щ�Ѿ���Ϊ��ģ���ڵ��׽��֡���ˣ����ǲ���ֱ��
	ʹ��socket���Ǽ�ӵļ���һ��Handle��������ʾ�ѱ��йܵ��׽�
	�֡�
*/
class TOOLLIB_API CNetEvent
{
public:
	CNetEvent( LONG nMaxSocketCount = 64 );
	~CNetEvent(void);

	BOOL	Connect( LPCSTR lpszHost, int nPort, DWORD dwTimeOut = 100 );
	VOID	Disconnect();
	INT		Send( const char* data, int len );
	bool	IsConnected()const { return m_bConnect; }

	bool	Start( int nPort );
	void	Stop();

protected:
	LONG	CreateSocketHandle( SOCKET s = INVALID_SOCKET );
	BOOL	CloseSocketHandle( LONG hHandle );
	BOOL	IsWork()const{ return m_bWork; }
	bool	IsValidSocketHandle( LONG hHandle )const{ return hHandle >= 0 && hHandle < m_nMaxSocketCount; }

	virtual ISocketBase* AllocClient() = 0;

	static unsigned int __stdcall EventThreadLoop( LPVOID lParam );

private:
	SOCKET*			m_pSocketArray;		// socket	����
	WSAEVENT*		m_pEventArray;		// Event	����
	ISocketBase**	m_pClientArray;		// Client	����
	LONG			m_nMaxSocketCount;	// ����Socket��������
	LONG			m_nListenIndex;		// �����������˿ڵ�������
	volatile LONG	m_nTotalNet;		// ��ǰ���ӵ�������
	bool			m_bConnect;			// �Ƿ�����

	HANDLE			m_hThread;			// �����߳̾��
	volatile LONG	m_bWork;			// �Ƿ��ڹ�����
	CCritical		m_Lock;				// ������
};

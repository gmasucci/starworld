#pragma once 

#define BUFFER_SIZE 512
#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

class TOOLLIB_API CNetSelect
{
public:
//------------------------------------------------------------------//
// ����
//------------------------------------------------------------------//
	enum 
	{
		SUCCESS		= 0x0000,
		RETRY		= 0x1000,
		DISCONNECT	= 0x1001,
		SOCKETERROR	= 0x1002,
	};

	CNetSelect();
	~CNetSelect();

	void			SendErrorCodeEx( int nErrorCode );
	BOOL			InitSocket();	// ��ʼ��Socket
	HRESULT			Connect( LPCSTR strHost, int nPort );	// ��������
	BOOL			Disconnect();

	HRESULT			Send( const char* pszBuffer, int size, int& nResult );
	HRESULT			Recv( char* pszBuffer, int size, int& nResult );

	int				GetLastError(){ return m_nErrorCode; }

protected:
	int				m_nErrorCode;
	SOCKET			m_socket;
};

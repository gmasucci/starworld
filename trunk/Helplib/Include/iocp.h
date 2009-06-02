#pragma once

#include <vector>
#include <queue>
#include <map>
#include <WinSock2.h>
#include <MSWSock.h>

#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

#define iocpEvent iocp_event
class iocp_event;
class ICompletionKey
{
public:
	//--------------------------------------------------------------------------------------------------------//
	// ��һ��Ͷ�����ʱ����
	// lpEvent			:	�¼�����ָ��
	// dwNumberOfByte	:	���������ݱ�����
	// return			:	�Ƿ��ͷ�Key
	//--------------------------------------------------------------------------------------------------------//
	virtual VOID OnEvent( iocpEvent* lpEvent, DWORD dwNumberOfByte ) = 0;

	//--------------------------------------------------------------------------------------------------------//
	// ���ü���
	//--------------------------------------------------------------------------------------------------------//
	virtual inline LONG AddRef() = 0;

	//--------------------------------------------------------------------------------------------------------//
	// �ͷ����ü���
	//--------------------------------------------------------------------------------------------------------//
	virtual inline LONG Release() = 0;

	//--------------------------------------------------------------------------------------------------------//
	// �õ����
	//--------------------------------------------------------------------------------------------------------//
	virtual inline HANDLE Get() = 0;

	//--------------------------------------------------------------------------------------------------------//
	// �رվ��
	//--------------------------------------------------------------------------------------------------------//
	virtual inline void Close() = 0;

};

//--------------------------------------------------------------------------------------------------------//
// �¼��ӿ�
//--------------------------------------------------------------------------------------------------------//
class TOOLLIB_API iocp_event	:	protected OVERLAPPED
{
public:
	explicit iocp_event( ICompletionKey* pKey, bool bEvent );
	virtual ~iocp_event();

	//--------------------------------------------------------------------------------------------------------//
	// ����Ҫ��Ϊ�˷�ֹǿ������ת������������
	//--------------------------------------------------------------------------------------------------------//
	operator LPOVERLAPPED() { return static_cast< OVERLAPPED* >( this ); }

	LPOVERLAPPED GetOverlapped() { return static_cast< LPOVERLAPPED >( this ); }

	//--------------------------------------------------------------------------------------------------------//
	// ����Ͷ�����ʱ����
	// pKey				:	�ĸ��ͻ��˵�Ͷ��
	// dwNumberOfByte	:	�ж������ݱ�����
	//--------------------------------------------------------------------------------------------------------//
	virtual void OnEvent( ICompletionKey* pKey, DWORD dwNumberOfByte ) = 0;
	virtual bool OnError( ICompletionKey* pKey, DWORD dwNumberOfByte, DWORD dwErrorCode ) = 0;
	virtual void Release() = 0;

private:
	ICompletionKey* m_pKey;
};

//--------------------------------------------------------------------------------------------------------//
// ��ɼ�
//--------------------------------------------------------------------------------------------------------//
class iocp_handle
{
public:
	virtual operator HANDLE()const = 0;
	virtual void close() = 0;

protected:
	bool closed;
};

class iocp_file	:	public iocp_handle
{
public:
	iocp_file( HANDLE handle ) : m_handle( handle ){ closed = false; }
	iocp_file( const iocp_file& handle ) : m_handle( handle.m_handle ){ closed = false; }

	virtual operator HANDLE()const { return m_handle; }
	HANDLE operator=( const HANDLE handle ) { m_handle = handle; return *this; }
	HANDLE operator=( const iocp_file& handle ) { m_handle = handle.m_handle; return *this; }

	void close()
	{ 
		if( !closed )
		{ 
			closed = true;
			CloseHandle( m_handle ); 
		}
	}

private:
	HANDLE m_handle;
};

class iocp_socket	:	public iocp_handle
{
public:
	iocp_socket( SOCKET handle ) : m_handle( handle ){ closed = false; }
	iocp_socket( const iocp_socket& handle ) : m_handle( handle.m_handle ){ closed = false; }

	operator SOCKET()const { return m_handle; }
	virtual operator HANDLE()const { return (HANDLE)m_handle; }
	SOCKET operator=( const SOCKET handle ) { m_handle = handle; return *this; }
	SOCKET operator=( const iocp_socket& handle ) { m_handle = handle.m_handle; return *this; }

	void close()
	{ 
		if( !closed )
		{
			closed = true;
			CancelIo( (HANDLE)m_handle );
			shutdown( m_handle, 0 );
			closesocket( m_handle );
		} 
	}

private:
	SOCKET m_handle;
};

template< class _HANDLE_T = iocp_file >
class CCompletionKey	: public ICompletionKey
{
public:
	CCompletionKey( CONST _HANDLE_T& hFile )
	: m_hFile( hFile )
	, m_ref( 1 )
	{
		m_data_vec.resize( 16, 0 );
	}

	virtual ~CCompletionKey()
	{
		Close();
	}

	//--------------------------------------------------------------------------------------------------------//
	// ��һ��Ͷ�����ʱ����
	// lpEvent			:	�¼�����ָ��
	// dwNumberOfByte	:	���������ݱ�����
	// return			:	�Ƿ��ͷ�Key
	//--------------------------------------------------------------------------------------------------------//
	virtual VOID OnEvent( iocpEvent* lpEvent, DWORD dwNumberOfByte )
	{
		if( lpEvent )
		{
			return lpEvent->OnEvent( this, dwNumberOfByte );
		}
	}

	//--------------------------------------------------------------------------------------------------------//
	// ȡ�����ݵ�����
	// index	:	����
	// remark	;
	/*
		������ΧҪ����0-15֮�䣬��ΪKeyֻ�ṩ��16�����ݴ������
	*/
	//--------------------------------------------------------------------------------------------------------//
	inline DWORD_PTR& operator[]( int index )
	{
		return m_data_vec[index];
	}

	inline DWORD_PTR& at( int index )
	{
		return m_data_vec[index];
	}

	//--------------------------------------------------------------------------------------------------------//
	// ��ֵ������
	//--------------------------------------------------------------------------------------------------------//
	inline void set_handle( CONST _HANDLE_T& handle )
	{
		m_hFile = handle;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �õ����
	//--------------------------------------------------------------------------------------------------------//
	virtual inline HANDLE Get()
	{
		return m_hFile;
	}

	inline CONST _HANDLE_T& get_handle()const
	{
		return m_hFile;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �رվ��
	//--------------------------------------------------------------------------------------------------------//
	inline void Close()
	{
		m_hFile.close();
	}

	//--------------------------------------------------------------------------------------------------------//
	// ���ü���
	//--------------------------------------------------------------------------------------------------------//
	inline LONG AddRef()
	{
		return InterlockedIncrement( &m_ref );
	}

	//--------------------------------------------------------------------------------------------------------//
	// �ͷ����ü���
	//--------------------------------------------------------------------------------------------------------//
	inline LONG Release()
	{
		ASSERT( m_ref != 0 );
		LONG ref = InterlockedDecrement( &m_ref );
		if( ref == 0 )
		{
			delete this;
		}
		return ref;
	}

protected:
	typedef std::vector< DWORD_PTR >	CDataVec;

	_HANDLE_T	m_hFile;
	CDataVec	m_data_vec;
	volatile LONG	m_ref;
};

//--------------------------------------------------------------------------------------------------------//
// �ṩһ�������������ģ��
//--------------------------------------------------------------------------------------------------------//
template< int Type, int BufferSize >
class iocpEventTemplate	:	public iocpEvent
{
public:
	enum { enType = Type }; // int to type

	explicit iocpEventTemplate( ICompletionKey* pKey, bool bEvent = false )
	: iocpEvent( pKey, bEvent )
	, m_buffer( new char[BufferSize] )
	, m_buffer_size( BufferSize )
	, m_cur_size( 0 )
	{
	}

	~iocpEventTemplate()
	{
		m_cur_size = 0;
		m_buffer_size = 0;
		delete[] m_buffer;
		m_buffer = 0;
	}

	//--------------------------------------------------------------------------------------------------------//
	// ���������뻺����
	// buf		:	������ַ
	// sizxe	:	���ݳ���
	// return	:	�Ѿ���������ݳ���
	//--------------------------------------------------------------------------------------------------------//
	size_t push( const char* buf, size_t size )
	{
		size_t read_size = min( m_buffer_size - m_cur_size, size );
		char* dest = m_buffer + m_cur_size;
		memcpy( dest, buf, read_size );
		m_cur_size += read_size;
		if( isdone() )
		{
			on_buffer_full();
		}
		return read_size;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �õ���������ַ
	//--------------------------------------------------------------------------------------------------------//
	char* get()const
	{
		return m_buffer;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �õ���ǰ���ݳ���
	// return	:	��ǰ���ݳ���
	//--------------------------------------------------------------------------------------------------------//
	size_t lenth()const
	{ 
		return m_cur_size;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �õ���������С
	//--------------------------------------------------------------------------------------------------------//
	size_t size()const
	{
		return m_buffer_size;
	}

	//--------------------------------------------------------------------------------------------------------//
	// �������Ƿ������
	//--------------------------------------------------------------------------------------------------------//
	virtual bool isdone()const
	{ 
		return m_cur_size == m_buffer_size;
	}

	//--------------------------------------------------------------------------------------------------------//
	// ���û�����ָ�뵽����ͷ
	//--------------------------------------------------------------------------------------------------------//
	void reset()
	{ 
		m_cur_size = 0;
	}

	// ƫ�ƻ�����ָ��
	size_t offset_pos( size_t offset )
	{
		size_t move = min( offset, m_buffer_size - m_cur_size );
		m_cur_size += move;
		return move;
	}

	//--------------------------------------------------------------------------------------------------------//
	// ��������֪ͨ
	//--------------------------------------------------------------------------------------------------------//
	virtual void on_buffer_full(){}
	
	virtual void Release()
	{
		delete this;
	}
private:
	char*	m_buffer;		// ������ָ��
	size_t	m_cur_size;		// ��ǰ�����
	size_t	m_buffer_size;	// ��������С
};

class TOOLLIB_API iocp
{
public:
	iocp(void);
	virtual ~iocp(void);

	typedef bool EnumComlationKey( HANDLE hHandle, ICompletionKey* pKey, LPVOID lpUserData );
	typedef unsigned __stdcall work( LPVOID Param );
	//--------------------------------------------------------------------------------------------------------//
	// ��ʼ����ɶ˿�
	// return	:	true ��ʼ���ɹ�, flase ��ʼ��ʧ��
	//--------------------------------------------------------------------------------------------------------//
	bool initialize();

	//--------------------------------------------------------------------------------------------------------//
	// �ر���ɶ˿�
	//--------------------------------------------------------------------------------------------------------//
	void uninitialize();
	
	//--------------------------------------------------------------------------------------------------------//
	// ��handle��������ɶ˿�
	// handle	�� ����������ɶ˿�
	// pKey		�� ��ɼ���ÿ��handle��һ����ɼ�
	// NumberOfConcurrentThreads	��	�����ɶ˿����������ͬʱ���ڵ��߳���
	//--------------------------------------------------------------------------------------------------------//
	bool associat( HANDLE handle, ICompletionKey* pKey = NULL, DWORD NumberOfConcurrentThreads  = 0 );

	//--------------------------------------------------------------------------------------------------------//
	// ���handle�Ĺ���
	// handle	:	�������ľ��
	// return	:	��handle��Ӧ��Key��ָ��
	//--------------------------------------------------------------------------------------------------------//
	ICompletionKey* disassociat( HANDLE handle );

	//--------------------------------------------------------------------------------------------------------//
	// ��ѯhandle�Ĺ��� ����AddRef ���ò�����
	// handle	:	�������ľ��
	// return	:	��handle��Ӧ��Key��ָ��
	//--------------------------------------------------------------------------------------------------------//
	ICompletionKey* query( HANDLE handle );

	//--------------------------------------------------------------------------------------------------------//
	// ö�ٵ�ǰ����ɶ˿ھ������ɼ�
	// FuncCallback	:	�ص�����ָ��
	// lpData		:	�û�����
	// return		:	�Ƿ����	true - ����	false - ��ֹ
	//--------------------------------------------------------------------------------------------------------//
	HANDLE enum_key( EnumComlationKey FuncCallback, LPVOID lpData );
	
	//--------------------------------------------------------------------------------------------------------//
	// ��ʼ�����߳�
	// dwNumberOfThread	:	�����߳����� default cpu���� * 2 + 2
	// pfn				:	�����̺߳���
	// lpParam			:	�����̲߳���
	//--------------------------------------------------------------------------------------------------------//
	bool start_iocp( DWORD dwNumberOfThread = 0, work pfn = default_work, LPVOID lpParam = NULL );

	//--------------------------------------------------------------------------------------------------------//
	// ֹͣ��ɶ˿�
	//--------------------------------------------------------------------------------------------------------//
	void stop_iocp();

	static unsigned __stdcall default_work( LPVOID lpParam );
	operator HANDLE();

public:
	enum worktype { TypeSingle, TypeMultiple }; // ������ʽ�� �����е��ߣ��Ͷ������ַ�ʽ��
	struct Event
	{
		iocpEvent*	pEvent;
		DWORD		dwNumberOfBytes;
	};

private:
	typedef std::vector< HANDLE >	CWorkThreadVec;
	typedef std::queue< Event* >	CEventQueue;
	typedef std::map< HANDLE, ICompletionKey* >	CKeyMap;

	struct impl;
	impl* m_impl;
};

class TOOLLIB_API iocp_exception
{
public:
	iocp_exception(){}
	virtual ~iocp_exception(){}
	virtual void do_exception() = 0;
};
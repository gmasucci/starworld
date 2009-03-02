#include "StdAfx.h"
#include "utility.h"
#include "logger.h"
#include "logger_adapter.h"
//--------------------------------------------------------------------------------------------------------//
// class CLogger 
// ��־�����ߣ�������¼���
//--------------------------------------------------------------------------------------------------------//
struct CLogger::IMPL
{
	typedef std::list< CLoggerAdapter* >	CLoggerAdapterList;
	typedef std::map< _string, logobj >		CLoggerInstanceMap;
	typedef std::list< _string >			CLoggerBufferList;

	CLoggerAdapterList			m_AdapterList;
	CLoggerBufferList			m_LoggerList;
	CRITICAL_SECTION			m_AdapterListLock;

	static CLoggerInstanceMap	m_LoggerMap;
	static CCritical			m_LoggerMapLock;

	logger_mode					m_mode;
};

CLogger::IMPL::CLoggerInstanceMap	CLogger::IMPL::m_LoggerMap;
CCritical							CLogger::IMPL::m_LoggerMapLock;

CLogger::CLogger( LPCTSTR szLoggerName )
: m_pImpl( new IMPL )
, m_strLoggerName( szLoggerName )
{
	InitializeCriticalSection( &m_pImpl->m_AdapterListLock );
	m_pImpl->m_mode = CLogger::normal;
}

CLogger::~CLogger(void)
{
	ASSERT( m_pImpl->m_AdapterList.empty() );
	DeleteCriticalSection( &m_pImpl->m_AdapterListLock );
	delete m_pImpl;
	m_pImpl = NULL;
}

//--------------------------------------------------------------------------------------------------------//
// ȡ����־ʵ��
// szLogger		:	ʵ����
//--------------------------------------------------------------------------------------------------------//
CLogger* CLogger::GetInstance( LPCTSTR szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );
	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.find( szLogger );
	CLogger* pLogger = NULL;
	if( citer == IMPL::m_LoggerMap.end() )
	{
		pLogger = new CLogger( szLogger );
		IMPL::m_LoggerMap[szLogger] = pLogger;
	}
	else
	{
		pLogger = citer->second;
	}
	return pLogger;
}

//--------------------------------------------------------------------------------------------------------//
// ������־���ģʽ
//--------------------------------------------------------------------------------------------------------//
VOID CLogger::SetLoggerMode( logger_mode en_mode )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.begin();
	while( citer != IMPL::m_LoggerMap.end() )
	{
		citer->second->SetMode( en_mode );
		++citer;
	}
}

//--------------------------------------------------------------------------------------------------------//
// ��־ʵ���Ƿ����
// szLogger		:	ʵ����
//--------------------------------------------------------------------------------------------------------//
bool CLogger::Have( LPCTSTR szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::const_iterator citer = IMPL::m_LoggerMap.find( szLogger );
	return citer != IMPL::m_LoggerMap.end();
}

//--------------------------------------------------------------------------------------------------------//
// �������
// szLogger		:	ʵ����
//--------------------------------------------------------------------------------------------------------//
void CLogger::Clear( LPCTSTR szLogger )
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::CLoggerInstanceMap::iterator iter = IMPL::m_LoggerMap.find( szLogger );
	if( iter != IMPL::m_LoggerMap.end() )
	{
		IMPL::m_LoggerMap.erase( iter );
	}
}

//--------------------------------------------------------------------------------------------------------//
// �����������
//--------------------------------------------------------------------------------------------------------//
void CLogger::ClearAll()
{
	CCriticalLock lock( IMPL::m_LoggerMapLock );

	IMPL::m_LoggerMap.clear();
}

//--------------------------------------------------------------------------------------------------------//
// ���һ������������
// pAdapter		:	������ָ��
//--------------------------------------------------------------------------------------------------------//
bool CLogger::AddAdapter( CLoggerAdapter* pAdapter )
{
	CCriticalLock lock( m_pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::const_iterator citer = std::find( m_pImpl->m_AdapterList.begin(), m_pImpl->m_AdapterList.end(), pAdapter );
	if( citer == m_pImpl->m_AdapterList.end() )
	{
		m_pImpl->m_AdapterList.push_back( pAdapter );
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------//
// ɾ��һ��������
// szAdapterName	:	��������
//--------------------------------------------------------------------------------------------------------//
CLoggerAdapter* CLogger::RemoveAdapter( LPCTSTR szAdapterName )
{
	CCriticalLock lock( m_pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::iterator iter		= m_pImpl->m_AdapterList.begin();
	IMPL::CLoggerAdapterList::iterator iter_end	= m_pImpl->m_AdapterList.end();
	CLoggerAdapter* pAdapter = NULL;
	while( iter != iter_end )
	{
		if( _tcscmp( szAdapterName, (*iter)->GetName() ) == 0 )
		{
			pAdapter = *iter;
			m_pImpl->m_AdapterList.erase( iter );
			break;
		}
		++iter;
	}
	return pAdapter;
}

//--------------------------------------------------------------------------------------------------------//
// д����
// szBuffer		:	���������������û��ṩ
// size			:	��������С
// fmt			:	��ʽ��
// ...			:	�����б�
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLog( LPTSTR szBuffer, size_t nSize, LPCTSTR szFmt, ... )
{
	ASSERT( this );
	ASSERT( szBuffer );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	va_list args;
	va_start(args, szFmt);

	int size = _vsntprintf( szBuffer, nSize, szFmt, args );
	szBuffer[nSize-1] = 0;
	if( m_pImpl->m_mode == normal )
		DispatchLog( szBuffer, size );
	else if( m_pImpl->m_mode == pause )
		m_pImpl->m_LoggerList.push_back( szBuffer );
	else
		return 0;
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// д����
// szBuffer		:	���������������û��ṩ
// ...			:	�����б�
// return		:	д����ַ���
// remark		:	С�ģ�����д��������ַ���255���ַ��պá�
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLog( LPCTSTR szFmt, ... )
{
	TCHAR tszLog[255];
	ASSERT( this );
	ASSERT( szFmt );
	if( m_pImpl->m_AdapterList.empty() ) return 0;

	va_list args;
	va_start(args, szFmt);

	size_t nSize = _countof( tszLog );
	int size = _vsntprintf( tszLog, nSize, szFmt, args );
	if( size < 0 )	return 0;
	tszLog[nSize-1] = 0;
	if( m_pImpl->m_mode == normal )
		DispatchLog( tszLog, size );
	else if( m_pImpl->m_mode == pause )
		m_pImpl->m_LoggerList.push_back( tszLog );
	else
		return 0;
	return size;
}

//--------------------------------------------------------------------------------------------------------//
// д����
// szLog		:	��־��
// nSize		:	������
//--------------------------------------------------------------------------------------------------------//
int CLogger::WriteLog( size_t nSize, LPCTSTR szLog )
{
	ASSERT( this );
	ASSERT( szLog );
	if( m_pImpl->m_mode == normal )
		DispatchLog( szLog, nSize );
	else if( m_pImpl->m_mode == pause )
		m_pImpl->m_LoggerList.push_back( szLog );
	else 
		return 0;
	return (int)_tcslen( szLog );
}

//--------------------------------------------------------------------------------------------------------//
// ������־���ģʽ
//--------------------------------------------------------------------------------------------------------//
VOID CLogger::SetMode( logger_mode en_mode )
{
	m_pImpl->m_mode = en_mode;
	if( en_mode == CLogger::normal )
	{
		IMPL::CLoggerBufferList::const_iterator citer = m_pImpl->m_LoggerList.begin();
		while( citer != m_pImpl->m_LoggerList.end() )
		{
			DispatchLog( citer->c_str(), citer->length() );
			++citer;
		}
		m_pImpl->m_LoggerList.clear();
	}
}

//--------------------------------------------------------------------------------------------------------//
// �ַ�������������
// szLog		:	��־��
// nSize		:	������
//--------------------------------------------------------------------------------------------------------//
void CLogger::DispatchLog( LPCTSTR szLog, size_t nSize )
{
	CCriticalLock lock( m_pImpl->m_AdapterListLock );

	IMPL::CLoggerAdapterList::const_iterator citer		= m_pImpl->m_AdapterList.begin();
	IMPL::CLoggerAdapterList::const_iterator citer_end	= m_pImpl->m_AdapterList.end();
	while( citer != citer_end )
	{
		(*citer)->WriteLog( m_strLoggerName, szLog, nSize );
		++citer;
	}
}

CLogger::logobj::~logobj()
{
	if( m_pLog )
	{ 
		delete m_pLog;
		m_pLog = NULL;
	}
}

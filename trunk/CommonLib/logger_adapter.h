#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

#ifndef _LOGGER_ADAPTER__H
#define _LOGGER_ADAPTER__H
#include "logger.h"


class EXPORT_LIB CLoggerAdapter
{
public:
	enum logger_level	{ debug, trace, warning, error, };

	CLoggerAdapter( _lpctstr strAdapterName, CLoggerAdapter::logger_level enLevel = CLoggerAdapter::debug )
		: m_strName( strAdapterName )
		, m_enLevel( enLevel )
	{}

	virtual ~CLoggerAdapter(){}

	//--------------------------------------------------------------------------------------------------------//
	// �õ�����������
	//--------------------------------------------------------------------------------------------------------//
	_lpctstr	GetName()const{ return m_strName.c_str(); }
	CLoggerAdapter::logger_level GetLevel()const{ return m_enLevel; }
	
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int Write( _lpcwstr szBuffer, size_t size ) = 0;

private:
	_string		m_strName;
	CLoggerAdapter::logger_level	m_enLevel;
};

//class CSocketAdapter	:	public CLoggerAdapter
//{
//public:
//	CSocketAdapter( _lpctstr szAdapterName, CLoggerAdapter::ENLEVEL enLevel = CLoggerAdapter::ADAPTER_DEBUG )
//		: CLoggerAdapter( szAdapterName, enLevel ){}
//	virtual ~CSocketAdapter();
//
//	//--------------------------------------------------------------------------------------------------------//
//	// ������־������
//	// szHost		:	��־������ip
//	// nPort		:	��־�������˿�
//	// return		:	�����Ƿ�ɹ�
//	//--------------------------------------------------------------------------------------------------------//
//	bool Connect( _lpctstr szHost, int nPort );
//
//	//--------------------------------------------------------------------------------------------------------//
//	// ����һ��Socket
//	// socket		:	ָ�����׽���
//	// return		:	�Ƿ�ɹ�
//	//--------------------------------------------------------------------------------------------------------//
//	bool AttachSocket( DWORD s );
//
//protected:
//	//--------------------------------------------------------------------------------------------------------//
//	// д��־
//	// szBuffer		:	��־�����������û��ṩ
//	// size			:	��������С
//	//--------------------------------------------------------------------------------------------------------//
//	virtual int WriteLog( _string strLoggerName, _lpctstr szBuffer, size_t size );
//
//private:
//	SOCKET	m_socket;
//};

class EXPORT_LIB CFileAdapter	:	public CLoggerAdapter
{
public:
	enum EN_FILEMODE
	{ 
		split_none	= 0,
		split_date	= 1,
		split_line	= 2,
	};
	//////////////////////////////////////////////////////////////////////////
	// ���캯��
	// szAdapterName	:	����������
	// nWriteVelocity	:	д�����ʣ�ÿ������ǿ��д��һ�� 0 Ϊ��ǿ��д�� 1 Ϊÿ�ζ�д�� ����ֵΪÿ������д��
	CFileAdapter( _lpctstr szAdapterName, int nWriteVelocity = 10, CLoggerAdapter::logger_level enLevel = CLoggerAdapter::debug )
		: CLoggerAdapter( szAdapterName, enLevel )
		, m_file( NULL )
		, m_nWriteVelocity( nWriteVelocity )
		, m_lpSplitParam( 0 )
		, m_nCurLine( 0 )
	{}
	virtual ~CFileAdapter();

	//--------------------------------------------------------------------------------------------------------//
	// ����־�ļ�
	// szFilename	:	��־�ļ���
	// enSplitMode	:	�ָ�ģʽ -split_date �����ڷָ� -split_line �������ָ� -split_none ���ָ�
	// lpParam		:	-split_line ģʽʱ �ò�������һ������ļ�����
	//					-split_date ģʽʱ �ò�������Ϊ 0
	// return		:	�ļ��Ƿ��
	//--------------------------------------------------------------------------------------------------------//
	bool Open( _lpctstr szFilename, _lpctstr szExt = _T("log"), EN_FILEMODE enSplitMode = split_none, LPARAM lpParam = 0 );

	//--------------------------------------------------------------------------------------------------------//
	// �ر���־�ļ�
	//--------------------------------------------------------------------------------------------------------//
	void Close();

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int Write( _lpcwstr szBuffer, size_t size );

	//--------------------------------------------------------------------------------------------------------//
	// ����SplitMode����һ���ļ���
	// lptszBuf		:	������
	// nCount		:	�������ַ�����
	// return		:	����˵Ļ������ַ�����
	//--------------------------------------------------------------------------------------------------------//
	int CombinedFileName( LPTSTR lptszBuf, size_t nCount );
private:
	_string		m_strFilename;
	_string		m_strExt;
	FILE*		m_file;
	int			m_nWriteVelocity;
	EN_FILEMODE	m_enSplitMode;
	LPARAM		m_lpSplitParam;
	UINT		m_nCurLine;
};

class EXPORT_LIB CWindowAdapter :	public CLoggerAdapter
{
public:
	explicit CWindowAdapter( HWND hEditHwnd, WPARAM wTextLimitLen = 1024*16, _lpctstr lpszName = _T("WindowLog") );
	virtual ~CWindowAdapter(void);

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int Write( _lpcwstr szBuffer, size_t size );

private:
	CONST HWND	m_hEditHwnd;
	WPARAM		m_nMaxLen;
	INT			m_nTotalLen;
};

class EXPORT_LIB CConsoleAdapter :	public CLoggerAdapter
{
public:
	explicit CConsoleAdapter( HANDLE hOutput, _lpctstr lpszName = _T("ConsoleLog"), WORD nCodePage = 936, CLoggerAdapter::logger_level enLevel = CLoggerAdapter::debug );
	virtual ~CConsoleAdapter(void);

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int Write( _lpcwstr szBuffer, size_t size );

private:
	HANDLE m_hOutput;
};

class EXPORT_LIB CDebugViewAdapter :	public CLoggerAdapter
{
public:
	explicit CDebugViewAdapter( _lpctstr lpszName = _T("DebugViewLog"), WORD nCodePage = 936, CLoggerAdapter::logger_level enLevel = CLoggerAdapter::debug );
	virtual ~CDebugViewAdapter(void);

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int Write( _lpcwstr szBuffer, size_t size );
};

#endif // _LOGGER_ADAPTER__H
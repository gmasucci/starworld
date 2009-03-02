#pragma once
#ifndef _LOGGER_ADAPTER__H
#define _LOGGER_ADAPTER__H

#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

class CLogger;
class TOOLLIB_API CLoggerAdapter
{
friend class CLogger;
public:
	enum ENLEVEL
	{ 
		ADAPTER_DEBUG,	// ���ԣ����Ի��������
		ADAPTER_ERROR,	// �����κλ��������
		ADAPTER_WRONG,	// ���棬ͨ��OutputDebugString��ʾ
	};
	CLoggerAdapter( LPCTSTR strAdapterName, CLoggerAdapter::ENLEVEL enLevel = CLoggerAdapter::ADAPTER_ERROR )
		: m_strName( strAdapterName ),
		m_enLevel( enLevel )
	{}

	virtual ~CLoggerAdapter(){}

	//--------------------------------------------------------------------------------------------------------//
	// �õ�����������
	//--------------------------------------------------------------------------------------------------------//
	LPCTSTR	GetName()const{ return m_strName.c_str(); }
	ENLEVEL GetLevel()const{ return m_enLevel; }
protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int WriteLog( _string strLoggerName, LPCTSTR szBuffer, size_t size ) = 0;

private:
	_string		m_strName;
	ENLEVEL		m_enLevel;
};

class TOOLLIB_API CSocketAdapter	:	public CLoggerAdapter
{
public:
	CSocketAdapter( LPCTSTR szAdapterName, CLoggerAdapter::ENLEVEL enLevel = CLoggerAdapter::ADAPTER_DEBUG )
		: CLoggerAdapter( szAdapterName, enLevel ){}
	virtual ~CSocketAdapter();

	//--------------------------------------------------------------------------------------------------------//
	// ������־������
	// szHost		:	��־������ip
	// nPort		:	��־�������˿�
	// return		:	�����Ƿ�ɹ�
	//--------------------------------------------------------------------------------------------------------//
	bool Connect( LPCTSTR szHost, int nPort );

	//--------------------------------------------------------------------------------------------------------//
	// ����һ��Socket
	// socket		:	ָ�����׽���
	// return		:	�Ƿ�ɹ�
	//--------------------------------------------------------------------------------------------------------//
	bool AttachSocket( DWORD s );

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int WriteLog( _string strLoggerName, LPCTSTR szBuffer, size_t size );

private:
	SOCKET	m_socket;
};

class TOOLLIB_API CFileAdapter	:	public CLoggerAdapter
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
	CFileAdapter( LPCTSTR szAdapterName, int nWriteVelocity = 10, CLoggerAdapter::ENLEVEL enLevel = CLoggerAdapter::ADAPTER_ERROR )
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
	bool Open( LPCTSTR szFilename, LPCTSTR szExt = _T("log"), EN_FILEMODE enSplitMode = split_none, LPARAM lpParam = 0 );

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
	virtual int WriteLog( _string strLoggerName, LPCTSTR szBuffer, size_t size );

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

class TOOLLIB_API CWindowAdapter :	public CLoggerAdapter
{
public:
	explicit CWindowAdapter( HWND hEditHwnd, LPCTSTR lpszName = _T("WindowLog") );
	virtual ~CWindowAdapter(void);

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int WriteLog( _string strLoggerName, LPCTSTR szBuffer, size_t size );

private:
	CONST HWND	m_hEditHwnd;
	INT			m_nTotalLen;
};

class TOOLLIB_API CConsoleAdapter :	public CLoggerAdapter
{
public:
	explicit CConsoleAdapter( LPCTSTR lpszName = _T("WindowLog"), CLoggerAdapter::ENLEVEL enLevel = CLoggerAdapter::ADAPTER_ERROR );
	virtual ~CConsoleAdapter(void);

protected:
	//--------------------------------------------------------------------------------------------------------//
	// д��־
	// szBuffer		:	��־�����������û��ṩ
	// size			:	��������С
	//--------------------------------------------------------------------------------------------------------//
	virtual int WriteLog( _string strLoggerName, LPCTSTR szBuffer, size_t size );
};

#endif // _LOGGER_ADAPTER__H
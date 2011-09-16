#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif


#ifndef _LOGGER_H
#define _LOGGER_H
#pragma warning( disable:4251 )
#include "defines.h"

namespace XGC
{
	namespace common
	{
		class CLoggerAdapter;

		class EXPORT_LIB CLogger
		{
		private:
			CLogger( _lpctstr szLoggerName );
			~CLogger(void);

		public:
			enum logger_mode	{ normal, pause, ignore };
			enum logger_level	{ debug, trace, warning, error, };
			enum header			{ header_date, header_time, header_thread, header_name, header_count, };

			//--------------------------------------------------------------------------------------------------------//
			// ȡ����־ʵ��
			// szLogger		:	ʵ����
			//--------------------------------------------------------------------------------------------------------//
			static CLogger* GetInstance( _lpctstr szLogger );

			static int WriteDateWidgetW( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );
			static int WriteTimeWidgetW( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );
			static int WriteNameWidgetW( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );
			static int WriteCountWidgetW( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );
			static int WriteThreadWidgetW( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );

			static int WriteDateWidgetA( CLogger* pLogger, _lpstr szBuffer, size_t nCount );
			static int WriteTimeWidgetA( CLogger* pLogger, _lpstr szBuffer, size_t nCount );
			static int WriteNameWidgetA( CLogger* pLogger, _lpstr szBuffer, size_t nCount );
			static int WriteCountWidgetA( CLogger* pLogger, _lpstr szBuffer, size_t nCount );
			static int WriteThreadWidgetA( CLogger* pLogger, _lpstr szBuffer, size_t nCount );

			//--------------------------------------------------------------------------------------------------------//
			// ������־���ģʽ
			//--------------------------------------------------------------------------------------------------------//
			static VOID SetLoggerMode( logger_mode en_mode );

			//--------------------------------------------------------------------------------------------------------//
			// ��־ʵ���Ƿ����
			// szLogger		:	ʵ����
			//--------------------------------------------------------------------------------------------------------//
			static bool	Have( _lpctstr szLogger );

			//--------------------------------------------------------------------------------------------------------//
			// �������
			// szLogger		:	ʵ����
			//--------------------------------------------------------------------------------------------------------//
			static void Clear( _lpctstr szLogger );

			//--------------------------------------------------------------------------------------------------------//
			// �����������
			//--------------------------------------------------------------------------------------------------------//
			static void ClearAll();

			//--------------------------------------------------------------------------------------------------------//
			// ���һ������������
			// pAdapter		:	������ָ��
			//--------------------------------------------------------------------------------------------------------//
			bool AddAdapter( CLoggerAdapter* pAdapter, bool bCaptrueDebugString = false );

			//--------------------------------------------------------------------------------------------------------//
			// ɾ��һ��������
			// szAdapterName	:	��������
			//--------------------------------------------------------------------------------------------------------//
			CLoggerAdapter* RemoveAdapter( _lpctstr szAdapterName );

			//---------------------------------------------------//
			// [3/4/2011 Albert]
			// Description:	��ȡ��־��ʽ 
			//---------------------------------------------------//
			int GetHeadW( _lpwstr szFmt, size_t nCount );


			//---------------------------------------------------//
			// [3/4/2011 Albert]
			// Description:	��ȡ��־��ʽ 
			//---------------------------------------------------//
			int GetHeadA( _lpstr szFmt, size_t nCount );

			//---------------------------------------------------//
			// [3/4/2011 Albert]
			// Description:	��ȡ��־��ʽ 
			//---------------------------------------------------//
			int GetHead( _lptstr szFmt, size_t nCount );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// size			:	��������С
			// fmt			:	��ʽ��
			// ...			:	�����б�
			// return		:	д����ַ���
			//--------------------------------------------------------------------------------------------------------//
			int WriteLogW( _lpwstr szBuffer, size_t nSize, _lpcwstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// ...			:	�����б�
			// return		:	д����ַ���
			// remark		:	С�ģ�����д�������ַ���4096���ַ��պá�
			//--------------------------------------------------------------------------------------------------------//
			int WriteLogW( _lpcwstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// size			:	��������С
			// fmt			:	��ʽ��
			// ...			:	�����б�
			// return		:	д����ַ���
			//--------------------------------------------------------------------------------------------------------//
			int WriteLogA( _lpstr szBuffer, size_t nSize, _lpcstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// ...			:	�����б�
			// return		:	д����ַ���
			// remark		:	С�ģ�����д�������ַ���4096���ַ��պá�
			//--------------------------------------------------------------------------------------------------------//
			int WriteLogA( _lpcstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// size			:	��������С
			// fmt			:	��ʽ��
			// ...			:	�����б�
			// return		:	д����ַ���
			//--------------------------------------------------------------------------------------------------------//
			int WriteLog( _lptstr szBuffer, size_t nCount, _lpctstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szBuffer		:	���������������û��ṩ
			// ...			:	�����б�
			// return		:	д����ַ���
			// remark		:	С�ģ�����д�������ַ���4096���ַ��պá�
			//--------------------------------------------------------------------------------------------------------//
			int WriteLog( _lpctstr szFmt, ... );

			//--------------------------------------------------------------------------------------------------------//
			// д����
			// szLog		:	��־��
			// nSize		:	������
			// return		:	д����ַ���
			//--------------------------------------------------------------------------------------------------------//
			int WriteData( const char* data, size_t size );

			//--------------------------------------------------------------------------------------------------------//
			// ������־���ģʽ
			// logger_mode	:	normal - ��ͨģʽ, pause - ��ͣģʽ, ignore - ����ģʽ };
			//--------------------------------------------------------------------------------------------------------//
			void SetMode( logger_mode en_mode );

			//--------------------------------------------------------------------------------------------------------//
			// ������־����ȼ�
			// logger_mode	:	debug - �������, trace - ����DebugOutputString���, warning - �������, error - �������
			//--------------------------------------------------------------------------------------------------------//
			void SetLevel( logger_level en_level );

			//---------------------------------------------------//
			// [3/4/2011 Albert]
			// Description:	������־ͷ 
			//---------------------------------------------------//
			void AddHeaderWidget( int (*pfnWriteW)( CLogger*, _lpwstr, size_t ), int (*pfnWriteA)( CLogger*, _lpstr, size_t ) );
		private:
			struct	IMPL;
			IMPL*	m_pImpl;

			struct HeaderWidget
			{
				int (*WriteW)( CLogger* pLogger, _lpwstr szBuffer, size_t nCount );
				int (*WriteA)( CLogger* pLogger, _lpstr szBuffer, size_t nCount );
			} m_HeaderWidget[8];

			class logobj
			{
			public:
				explicit logobj(): m_pLog( NULL ){}
				explicit logobj( CLogger* pLog ): m_pLog( pLog ){}
				explicit logobj( const logobj& obj ){ m_pLog = obj.m_pLog; }

				~logobj();
				CLogger* operator=( CLogger* pLog ){ m_pLog = pLog;	return m_pLog; }
				CLogger* operator->()const{ return m_pLog; }
				operator CLogger*()const{	return m_pLog; }

			private:
				CLogger* m_pLog;
			};

		protected:
			//--------------------------------------------------------------------------------------------------------//
			// �ַ�������������
			// szLog		:	��־��
			// nSize		:	������
			//--------------------------------------------------------------------------------------------------------//
			static void DispatchLog( CLogger::IMPL* pImpl, void *data, size_t size );

			static unsigned int __stdcall captrue_debug_thread( void *param );
			static unsigned int __stdcall writer_thread( void *param );
		};
	}
}
#endif // _LOGGER_H
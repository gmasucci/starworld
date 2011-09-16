#pragma once
#ifndef _COMMON_H
#define _COMMON_H

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif
namespace XGC
{
	namespace common
	{
		//--------------------------------------------------------------------------------------------------------//
		// ���·������·�����Ϊ����·�� ������"E:\tools\Downloads\"
		// ��񻯺��·������������·����ֻ�ж�·���Ƿ�Ϸ�������������·�����Ե�ǰ�������ڵ�Ŀ¼Ϊ��ǰĿ¼
		// Ŀ¼��񻯺���'\'��β��
		// [in]lpszPath				:	��Ҫ��񻯵�·��
		// [out]lpszNormalizePath	:	��񻯺�����������
		// [in]dwSize				:	�����������С
		//--------------------------------------------------------------------------------------------------------//
		EXPORT_LIB bool NormalizePath( _lpctstr lpszPath, LPTSTR lpszNormalizePath, DWORD dwSize );

		// �ж��Ƿ�UTF8�ı�
		EXPORT_LIB _int32 IsTextUTF8( LPSTR lpstrInputStream, INT iLen );

		// ΪAPI�򲹶���IAT��
		EXPORT_LIB bool patchimport( HMODULE importmodule, _lpcstr exportmodulename, _lpcstr exportmodulepath, _lpcstr importname, _lpcvoid replacement );

		// �������
		EXPORT_LIB void restoreimport (HMODULE importmodule, _lpcstr exportmodulename, _lpcstr exportmodulepath, _lpcstr importname, _lpcvoid replacement);

		//--------------------------------------------------------//
		//	created:	3:12:2009   16:04
		//	filename: 	utility
		//	author:		Albert.xu
		//
		//	purpose:	��ȡ���һ��ϵͳ���������
		//--------------------------------------------------------//
		EXPORT_LIB _lpctstr GetErrorDescription();

		// ��ת��
		EXPORT_LIB _lpcstr ws2s(const std::wstring& ws);
		EXPORT_LIB _lpcstr ws2s(const wchar_t* ws);

		EXPORT_LIB _lpcwstr s2ws(const std::string& s);
		EXPORT_LIB _lpcwstr s2ws(const char* s);

		#ifdef _UNICODE
			#define XA2T( a )	s2ws( a )
			#define XT2A( t )	ws2s( t )
			#define XW2T( w )	w
			#define XT2W( t )	t

		#else
			#define XA2T( a )	a
			#define XT2A( t )	t
			#define XW2T( w )	ws2s( a )
			#define XT2W( t )	s2ws( t )

		#endif
			#define XA2W( a )	s2ws( a )
			#define XW2A( w )	ws2s( w )

		EXPORT_LIB _lpcstr gethost( _lpcstr pszAny );

		//--------------------------------------------------------------------------------------------------------//
		// D2Hex
		// pData		:	������ַ
		// dwDataSize	:	���ݳ��ȣ����ַ��ƣ�
		// pOut			:	���������ַ
		// dwOutSize	:	���������ȣ����ַ��ƣ�
		// dwFlags		:	��־ ��δ���壩
		// return		:	����ת���˶����ַ�
		//--------------------------------------------------------------------------------------------------------//
		#define HEX_WIDTH_8		0x00000000
		#define HEX_WIDTH_16	0x00000001
		#define HEX_WIDTH_32	0x00000002
		#define HEX_WIDTH_64	0x00000003

		EXPORT_LIB _uint32 data2hex( _lpcstr pData, _uint32 nDataSize, _lptstr pOut, _uint32 nOutSize, _uint32 nFlags = 0 );

		EXPORT_LIB char* strntok( char *string, const char *end, const char *control, char **next );

		EXPORT_LIB bool compare_wildcard(const char* str1, const char* comaprestr);
		//--------------------------------------------------------------------------------------------------------//
		#define INET_SERVICE_UNK        0x1000
		//--------------------------------------------------------------------------------------------------------//
		// inet ��������
		#if defined( _USE_WININET )
		BOOL EXPORT_LIB ParseURLEx(_lpctstr pstrURL, DWORD& dwServiceType, _string& strServer, _string& strObject, INTERNET_PORT& nPort, _string& strUsername, _string& strPassword, DWORD dwFlags/* = 0*/);
		BOOL EXPORT_LIB ParseURL(_lpctstr pstrURL, DWORD& dwServiceType, _string& strServer, _string& strObject, INTERNET_PORT& nPort);
		#endif
	}
}
#endif // _COMMON_H
#pragma once
#ifndef _COMMON_H
#define _COMMON_H

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif


inline EXPORT_LIB BOOL IsValidString(LPCWSTR psz, size_t nMaxLength = INT_MAX)
{
#if defined(_DEBUG)
	// Implement ourselves because ::IsBadStringPtrW() isn't implemented on Win9x.
	if ((psz == NULL) || (nMaxLength == 0))
		return FALSE;

	LPCWSTR pch;
	LPCWSTR pchEnd;
	__try
	{
		wchar_t ch;

		pch = psz;
		pchEnd = psz+nMaxLength-1;
		ch = *(volatile wchar_t*)pch;
		while ((ch != L'\0') && (pch != pchEnd))
		{
			pch++;
			ch = *(volatile wchar_t*)pch;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return FALSE;
	}

	return TRUE;
#else
	(nMaxLength);
	return (psz != NULL);
#endif	
}

// Verify that a null-terminated string points to valid memory
inline EXPORT_LIB BOOL IsValidString(LPCSTR psz, size_t nMaxLength = UINT_MAX)
{
#if defined(_DEBUG)
	if (psz == NULL)
		return FALSE;
	return ::IsBadStringPtrA(psz, nMaxLength) == 0;
#else
	(nMaxLength);
	return (psz != NULL);
#endif
}

// Verify that a pointer points to valid memory
inline EXPORT_LIB BOOL IsValidAddress(const void* p, size_t nBytes,BOOL bReadWrite = TRUE)
{
#if defined(_DEBUG)
	return ((p != NULL) && !IsBadReadPtr(p, nBytes) &&
		(!bReadWrite || !IsBadWritePtr(const_cast<LPVOID>(p), nBytes)));
#else
	nBytes,bReadWrite;
	return (p != NULL);
#endif
}

template<typename T>
	inline void AssertValidObject(const T *pOb)
{
	ATLASSERT(pOb);
	ATLASSERT(AtlIsValidAddress(pOb, sizeof(T)));
	if(pOb)
		pOb->AssertValid();
}

// ��ջ������
#define RTCHECK() debug_helper::RTCCheck __FILE__##__LINE__()
class EXPORT_LIB RTCCheck
{
public:
	RTCCheck();
	~RTCCheck();
private:
	int m_checkValue;
};

//--------------------------------------------------------------------------------------------------------//
// ��ת�����ֽ�˳��
// src Դ����
//--------------------------------------------------------------------------------------------------------//
template< typename T >
T ReversalValue( T src )
{
	T value = 0;
	size_t size = sizeof( T );
	char* pDest = reinterpret_cast< char* >( &value );
	char* pSrc = reinterpret_cast< char* >( &src );
	pSrc += size;
	while( size-- )	*pDest++ = *--pSrc;
	return value;
}

EXPORT_LIB bool GetModulePath( HMODULE hModule, TCHAR* pszPath, DWORD nOutSize );

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

class EXPORT_LIB CErrorDescription
{
public:
	CErrorDescription();
	~CErrorDescription();

	_lpctstr GetDescription();
	DWORD	GetErrorCode();

private:
	DWORD	dwErrorCode;
	LPVOID	lpszDescription;
};

// ��ת��
EXPORT_LIB std::string ws2s(const std::wstring& ws);
EXPORT_LIB std::string ws2s(const wchar_t* ws);

EXPORT_LIB std::wstring s2ws(const std::string& s);
EXPORT_LIB std::wstring s2ws(const char* s);

#ifdef _UNICODE
	#define XA2T( a )	s2ws( a ).c_str()
	#define XT2A( t )	ws2s( t ).c_str()
	

	#define XW2T( w )	w
	#define XT2W( t )	t

	#define XA2TSTR( a )	s2ws( a )
	#define XT2ASTR( t )	ws2s( t )
	#define XW2TSTR( w )	std::wstring( w )
	#define XT2WSTR( t )	std::wstring( t )

#else
	#define XA2T( a )	a
	#define XT2A( t )	t
	#define XW2T( w )	ws2s( a ).c_str()
	#define XT2W( t )	s2ws( t ).c_str()

	#define XA2TSTR( a )	std::string( a )
	#define XT2ASTR( t )	std::string( t )
	#define XW2TSTR( w )	ws2s( w )
	#define XT2WSTR( t )	s2ws( t )
#endif
	#define XA2W( a )	s2ws( a ).c_str()
	#define XW2A( w )	ws2s( w ).c_str()

#endif // _COMMON_H
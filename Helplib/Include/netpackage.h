#pragma once

#ifdef TOOLLIB_EXPORTS
#define TOOLLIB_API __declspec(dllexport)
#else
#define TOOLLIB_API __declspec(dllimport)
#endif	//TOOLLIB_EXPORTS

class TOOLLIB_API CNetPackage
{
public:
	CNetPackage( size_t package_size );
	~CNetPackage(void);

public:
	//--------------------------------------------------------------------------------------------------------//
	// �����趨�ĳ��Ƚ�ȡ��Ϣ����
	// pbuf	:	����Ļ�������ַ
	// size	:	����������
	// return	:	�ж����ֽڱ��洢
	//--------------------------------------------------------------------------------------------------------//
	const char* get()const	{ return m_pbuf; }
	size_t	push( const char* pbuf, size_t size );
	size_t	length()const	{ return m_package_size - m_release_len; }
	size_t	size()const		{ return m_package_size; }
	bool	is_done()const	{ return m_release_len == 0; }
	size_t	reset()			{ m_release_len = m_package_size; return m_package_size; }
protected:
	size_t	m_package_size;		// ��Ϣ������
	size_t	m_release_len;		// δ���ĳ���
	char*	m_pbuf;				// ��Ϣ���׵�ַ

};

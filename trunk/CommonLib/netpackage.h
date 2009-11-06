#pragma once

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

class EXPORT_LIB CNetPackage
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
	void*	detach()		{ void* data = (void*)m_pbuf; m_pbuf = NULL; return data;}
protected:
	size_t	m_package_size;		// ��Ϣ������
	size_t	m_release_len;		// δ���ĳ���
	char*	m_pbuf;				// ��Ϣ���׵�ַ

};

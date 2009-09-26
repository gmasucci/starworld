#include "StdAfx.h"
#include "netstream.h"

CNetInStream::CNetInStream(void)
{
	m_pbuf		= NULL;
	m_nBufSize	= 0;
	m_nNowSite	= 0;
	m_nLength	= 0;
}

CNetInStream::CNetInStream( char* data, size_t size )
: m_pbuf( data )
, m_nBufSize( size )
, m_nNowSite( 0 )
, m_nLength( 0 )
{
}

CNetInStream::~CNetInStream(void)
{
}

// �ú�����Ҫ���ڽ��ջ�����
// pNewBuffer	:	�µĲ�������
// nSize		:	��������С
// return		:	�ɵĻ�������ַ
char* CNetInStream::attach( char* pNewBuffer, size_t nSize )
{
	char* pbuf	= m_pbuf;
	m_pbuf		= pNewBuffer;
	m_nBufSize	= nSize;
	m_nNowSite	= 0;
	return pbuf;
}

// ���뻺����
// return	:	��ǰ�Ļ�����ָ��
char* CNetInStream::detach()
{
	m_nBufSize = 0;
	m_nNowSite = 0;
	char* pbuf = m_pbuf;
	return pbuf;
}

// �ӵ�ǰλ�ø��ӻ�����
// pszbuf	:	���ӵĻ�������ַ
// size		:	���ӻ������Ĵ�С
CNetInStream& CNetInStream::write( const char* pszbuf, size_t size )
{
	if( size > m_nBufSize - m_nNowSite )
	{
		throw( _T("buffer over length.") );
	}

	if( m_pbuf + m_nNowSite != pszbuf )
	{
		memcpy( m_pbuf + m_nNowSite, pszbuf, size );
	}
	m_nNowSite += size;
	return *this;
}

// ��λ������ƫ����
VOID CNetInStream::reset()
{
	m_nNowSite = 0;
}

bool CNetInStream::set( size_t pos )
{ 
	if( pos < 0 || pos >= m_nBufSize )
	{
		return false;
	}
	else
	{
		m_nNowSite = pos;
		return true;
	}
}

bool CNetInStream::offset( size_t delta )
{ 
	if( m_nNowSite + delta < 0 || m_nNowSite + delta > m_nBufSize )
	{
		return false;
	}
	else
	{
		m_nNowSite += delta;
		return true;
	}
}

void CNetInStream::end()
{
	m_nLength = m_nNowSite;
}

//////////////////////////////////////////////////////////////////////////
// OutStream
//////////////////////////////////////////////////////////////////////////

CNetOutStream::CNetOutStream( const char* data, size_t size )
:m_pbuf( data )
,m_nBufSize( size )
,m_nNowSite( 0 )
{
}

// ��λ������ƫ����
void CNetOutStream::reset()
{
	m_nNowSite = 0;
}

size_t	CNetOutStream::read( char* buf, size_t size )
{
	size_t cpysize = min( m_nBufSize - m_nNowSite, size );
	memcpy( buf, m_pbuf + m_nNowSite, cpysize );
	m_nNowSite += cpysize;
	return cpysize;
}

// move pos to
bool CNetOutStream::setpos( size_t pos )
{ 
	if( pos < 0 || pos >= m_nBufSize )
		return false;
	else
	{
		m_nNowSite = pos;
		return true;
	}
}

bool CNetOutStream::offset( size_t delta )
{ 
	if( m_nNowSite + delta < 0 || m_nNowSite + delta > m_nBufSize )
		return false;
	else
	{
		m_nNowSite += delta;
		return true;
	}
}

#include "StdAfx.h"
#include "dynamic_buffer.h"

//--------------------------------------------------------------------------------------------------------//
// CDynamicBuffer
// ��̬������������ҪԤ�ȷ��仺������С
//--------------------------------------------------------------------------------------------------------//
CDynamicBuffer::CDynamicBuffer( DWORD chunksize/* = 4 * 1024*/ )
: m_chunksize( chunksize )
, m_head( (UCHAR*)malloc(chunksize) )
, m_headsize( chunksize )
, m_size( 0 )
{
	memset( m_head, 0, chunksize );
}

CDynamicBuffer::~CDynamicBuffer()
{
	CChunkVec::const_iterator citer = m_chunkvec.begin();
	while( citer != m_chunkvec.end() )
	{
		delete[] *citer;
		++citer;
	}

	if( m_head )
	{
		free( m_head );
		m_head = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------//
// ����Chunk��С
// dwSize	:	�趨�Ĵ�С
//--------------------------------------------------------------------------------------------------------//
VOID CDynamicBuffer::SetChunkSize( DWORD dwSize )
{
	CleanUp();
	m_chunksize = dwSize;
}

//--------------------------------------------------------------------------------------------------------//
// �õ���������ַ
// return	:	��������ַ
//--------------------------------------------------------------------------------------------------------//
LPBYTE CDynamicBuffer::Get()
{
	CleanUp( TRUE );
	return m_head;
}

//--------------------------------------------------------------------------------------------------------//
// �õ�����������
// return	:	����������
//--------------------------------------------------------------------------------------------------------//
ULONG CDynamicBuffer::Size()const
{
	return m_size;
}

//--------------------------------------------------------------------------------------------------------//
// ��仺����
// offset	:	������ƫ����
// buf		:	��Ҫ����Ļ�����
// size		:	��������С
// return	:	ʵ��д����ֽ���
//--------------------------------------------------------------------------------------------------------//
ULONG	CDynamicBuffer::Fill( ULONG offset, CONST LPBYTE buf, ULONG size )
{
	ULONG write_count = 0;

	// Buffer ����ʼ������head��ָ�Ļ������У����⣬
	if( offset < m_headsize )
	{
		ULONG release = m_headsize - offset;
		ULONG write = min( release, size );
		UCHAR* head_buf = m_head + offset;
		memcpy( head_buf, buf + write_count, write );

		write_count += write;
		size -= write;
		offset += write;
	}

	while( size )
	{
		ULONG chunkindex = (offset-m_headsize)/m_chunksize;
		if( m_chunkvec.size() <= chunkindex )
		{
			m_chunkvec.resize( chunkindex + 1, NULL );
		}

		UCHAR* chunk_buf = m_chunkvec.at( chunkindex );
		if( chunk_buf == NULL )
		{
			chunk_buf = new UCHAR[m_chunksize];
			m_chunkvec.at( chunkindex ) = chunk_buf;
		}

		ULONG release = m_chunksize - ((offset-m_headsize)%m_chunksize);

		ULONG write =  min( release, size );

		chunk_buf += (offset-m_headsize) % m_chunksize;
		memcpy( chunk_buf, buf + write_count, write );

		write_count += write;
		size -= write;
		offset += write;
	}
	m_size = max( m_size, offset + size );
	return write_count;
}

//--------------------------------------------------------------------------------------------------------//
// ����������
//--------------------------------------------------------------------------------------------------------//
void CDynamicBuffer::CleanUp( BOOL bFree /*= FALSE*/ )
{
	ULONG size = (ULONG)m_chunkvec.size();
	ULONG inc = size * m_chunksize;
	if( inc > 0 )
	{
		size_t offset = m_headsize;
		m_headsize += inc;
		LPBYTE buf = (LPBYTE)realloc( m_head, m_headsize );

		ASSERT( buf );
		if( buf )
		{
			m_head	= buf;

			buf += offset;
			CChunkVec::iterator iter = m_chunkvec.begin();
			while( m_chunkvec.end() != iter )
			{
				if( *iter == NULL )
				{
					memset( buf, 0, m_chunksize );
				}
				else
				{
					memcpy( buf, *iter, m_chunksize );
				}
				buf += m_chunksize;
				if( bFree ) delete[] *iter;
				++iter;
			}
			if( bFree )	m_chunkvec.clear();
		}
	}
}
//--------------------------------------------------------------------------------------------------------//


#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

#pragma warning( disable:4251 )
class EXPORT_LIB CDynamicBuffer
{
public:
	CDynamicBuffer( DWORD chunksize = 4 * 1024 );
	~CDynamicBuffer();
	//--------------------------------------------------------------------------------------------------------//
	// �õ���������ַ
	// return	:	��������ַ
	//--------------------------------------------------------------------------------------------------------//
	LPBYTE	Get();

	//--------------------------------------------------------------------------------------------------------//
	// �õ�����������
	// return	:	����������
	//--------------------------------------------------------------------------------------------------------//
	ULONG	Size()const;

	//--------------------------------------------------------------------------------------------------------//
	// ��仺����
	// offset	:	������ƫ����
	// buf		:	��Ҫ����Ļ�����
	// size		:	��������С
	// return	:	ʵ��д����ֽ���
	//--------------------------------------------------------------------------------------------------------//
	ULONG	Fill( ULONG offset, CONST LPBYTE buf, ULONG size );

	//--------------------------------------------------------------------------------------------------------//
	// �������
	// offset	:	������ƫ����
	// buf		:	ֵ
	// bReversal:	�Ƿ���
	// return	:	ʵ��д����ֽ���
	//--------------------------------------------------------------------------------------------------------//
	template< typename T >
	ULONG FillType( ULONG offset, T buf, bool bReversal = false )
	{
		//if( bReversal )
		//{
		//	buf = helper::ReversalValue( buf );
		//}
		return Fill( offset, (CONST LPBYTE)&buf, sizeof( T ) );
	}

	//------------- Warning ---------------//
	// ���ʹ��ָ������ ������һ�������ڴ���, ���������㣬 ʹ��ָ���ǷǷ��ģ���
	// ���������ԭ������Ϊû�з���ֵ
	//--------------------------------------------------------------------------------------------------------//
	template< typename T >
	ULONG FillType( ULONG offset, T* buf, bool bReversal = true ){}

protected:
	void CleanUp();	

private:
	typedef std::vector< UCHAR* >	CChunkVec;

	CChunkVec	m_chunkvec;		// ��16kΪһ������ɵĻ������б�
	UCHAR*		m_head;			// �����壬����chunklist�����еĻ������ݱ�������ͷ��ָ�Ļ������С�
	ULONG		m_headsize;		// ����������С
	ULONG		m_size;			// ��Ч����Ĵ�С��
	DWORD		m_chunksize;	// ���С
};

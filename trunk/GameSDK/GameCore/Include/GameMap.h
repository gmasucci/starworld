#pragma once
#include "GameObject.h"
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#define BLOCK_ADD		1
#define BLOCK_DEL		2
#define BLOCK_EXCANGE	3
//////////////////////////////////////////////////////////////////////////
// MapBlock ���ӹ���
class CORE_API CMapBlock
{
public:
	bool	m_bAllow;			// �Ƿ�����ͨ��
	float	m_fHeight;			// �߶�

	typedef std::set< _uint32, LESS< _uint32 >, ALLOCATOR< _uint32 > >	CObjList;
	CObjList m_ObjList;

	CMapBlock()
	{
	}

	~CMapBlock()
	{
	};

	DEBUG_CODE(	bool CheckExist( _uint32 nID )const{ return m_ObjList.find( nID ) != m_ObjList.end(); })
	//////////////////////////////////////////////////////////////////////////
	// ObjectManage
	// ��Ӷ�������

	void AddObject( _uint32 nID )
	{
		m_ObjList.insert( nID );
	}

	size_t DelObject( _uint32 nObjID )
	{
		return m_ObjList.erase( nObjID );
	}

	template< class func >
	void for_each_object( func &pfnCall )
	{
		std::for_each( m_ObjList.begin(), m_ObjList.end(), pfnCall );
	}

	bool Empty()const{ return m_ObjList.empty(); }
};

class CORE_API CGameMap	: public CXObject
{
private:
	// ��ͼ
	CMapBlock*	m_pBlockArray;

protected:
	xgcSize	m_siMapSize;
	xgcSize	m_siBlockSize;
	xgcPoint m_ptTransfrom;
	
	float m_InvBW, m_InvBH;
public:
	typedef std::list< CMapBlock*, ALLOCATOR< CMapBlock* > >	CBlockList;
	typedef std::set< _uint32, LESS< _uint32 >, ALLOCATOR< _uint32 > >			CTargetSet;
	typedef std::function< bool( CMapBlock* ) > fnBlockCallback;

protected:
	CGameMap()
		: m_siBlockSize( xgcSize( 0, 0 ) )
		, m_siMapSize( xgcSize( 0, 0 ) )	// ��ש�������
		, m_pBlockArray( NULL )
		, m_InvBW( 0.0f )
		, m_InvBH( 0.0f )
	{
		// ��ͼ������Ϣ
	}

	CGameMap( bool bIsParent, bool bIsTypeList )
		: CXObject( bIsParent, bIsTypeList )
		, m_siBlockSize( xgcSize( 0, 0 ) )
		, m_siMapSize( xgcSize( 0, 0 ) )	// ��ש�������
		, m_pBlockArray( NULL )
		, m_InvBW( 0.0f )
		, m_InvBH( 0.0f )
	{
		// ��ͼ������Ϣ
	}

	virtual ~CGameMap()
	{
		DestroyMap();
	}

	// ��ʼ����ͼ
	void InitializeMap( _uint32 nBlockWidth, _uint32 nBlockHeight, SIZE siBlockSize, POINT ptTransfrom );
	void DestroyMap();

public:
	DECLARE_DYNAMICTYPE( CXObject, TypeGameMap );

	inline	_uint32	GetMapWidth()const		{ return m_siMapSize.cx; }
	inline	_uint32	GetMapHeight()const		{ return m_siMapSize.cy; }
	inline	_uint32	GetBlockWidth()const	{ return m_siBlockSize.cx; }
	inline	_uint32	GetBlockHeight()const	{ return m_siBlockSize.cy; }
	inline	_uint32	GetWorldWidth()const	{ return m_siMapSize.cx*m_siBlockSize.cx; }
	inline	_uint32	GetWorldHeight()const	{ return m_siMapSize.cy*m_siBlockSize.cy; }

	inline	CMapBlock*	GetBlock( int x, int y )const;

	xgcRect	MapToWorld( const xgcRect& rc )const{ return rc + m_ptTransfrom; }
	xgcRect WorldToMap( const xgcRect& rc )const{ return rc - m_ptTransfrom; }

	xgcPoint MapToWorld( const xgcPoint& pt )const{ return pt + m_ptTransfrom; }
	xgcPoint WorldToMap( const xgcPoint& pt )const{ return pt - m_ptTransfrom; }

	//---------------------------------------------------//
	// [9/10/2009 Albert]
	// Description:	��ȡ���η�Χ�ڵĸ����б�
	//---------------------------------------------------//
	int	GetBlockList( const xgcRect& rcBlockArea, CBlockList& ret )const;

	//---------------------------------------------------//
	// [9/3/2009 Albert]
	// Description:	��ȡֱ���ϵĸ����б�
	//---------------------------------------------------//
	int GetBlockList( xgcPoint ptBegin, xgcPoint ptEnd, int nWidth, CBlockList& ret )const;

	//---------------------------------------------------//
	// [9/3/2009 Albert]
	// Description:	��ȡ�����ڵ����ж���ID
	//---------------------------------------------------//
	static
	int	GetTargets( const CBlockList& bl, CTargetSet& ret );


	//---------------------------------------------------//
	// [8/7/2009 Albert]
	// Description:	�ƶ�����
	//---------------------------------------------------//
	bool DynamicMoveTo( CDynamicObject* Obj, XVector3 &vPosition, bool bCollisionTest = true );

	//---------------------------------------------------//
	// [8/7/2009 Albert]
	// Description:	��ȡ����ռ�õĸ��ӷ�Χ
	//---------------------------------------------------//
	xgcRect	GetBlockArea( const xgcRect& rcCollision )const;

	//---------------------------------------------------//
	// [8/7/2009 Albert]
	// Description:	��ȡ����ռ�õĸ��ӷ�Χ
	//---------------------------------------------------//
	xgcRect	GetBlockArea( float fPosX, float fPosY, float fRadius )const;
protected:

	//---------------------------------------------------//
	// [8/6/2009 Albert]
	// Description:	����ӽڵ�ǰ������
	//---------------------------------------------------//
	virtual bool PreAddChild( CXObject* pObj );	// ����������ͼ

	//---------------------------------------------------//
	// [8/6/2009 Albert]
	// Description:	ɾ���ӽڵ�ǰ������
	//---------------------------------------------------//
	virtual bool PreRemoveChild( CXObject* pObj, bool bRelease );

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	��ײ���
	//---------------------------------------------------//
	virtual bool TestCollision( CMapBlock* pBlock, CGameObject *pObj );

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	��ɫ�ƶ������
	// pObj		:	�ƶ��Ķ���
	// nPosX, nPosY, nPosZ:	Ŀ�������
	//---------------------------------------------------//
	virtual void OnDynamicMove( CDynamicObject *pObj, XVector3 &vPosition );

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	������ռ���ӷ����ı�ʱ����
	//---------------------------------------------------//
	virtual void ExchangeBlock( CDynamicObject* pObj, const xgcPoint *pOldBlock, const xgcPoint *pNewBlock );
};

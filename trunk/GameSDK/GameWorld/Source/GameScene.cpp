#include "StdAfx.h"
#include "GameScene.h"
#include "ResourceMng.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////
// CMap
// ��ʼ����ͼ
VOID CMap::InitializeMap( uint32 dwMapWidth, uint32 dwMapHeight, int nRoleID )
{
	m_pBlockArray = new CMapBlock[dwMapWidth*dwMapHeight];

	m_dwMapWidth	= dwMapWidth;
	m_dwMapHeight	= dwMapHeight;

	if( CXObjectList::GetInstance().IsValidID( nRoleID ) )
	{
		m_nRoleID = nRoleID;
		PutObjToMap( nRoleID );
	}
}

inline CMapBlock* CMap::GetBlock( uint32 x, uint32 y )const
{
	if( x < 0 || x >= m_dwMapWidth || y < 0 || y >= m_dwMapHeight )	return NULL;

	return m_pBlockArray?m_pBlockArray + (y*m_dwMapWidth+x):NULL;
}

struct EnumChildParam
{
	CMap*	pMap;
	float	fDeltaTime;
};

uint32	CMap::Update( float fDeltaTime )
{
	// �߼����ƣ�����
	// ��֡��������֡������Animation�е�AniSpeed���ơ�
	// ��ͼ������߼�
	CAnimationList::const_iterator citer	= m_AnimationList.begin();
	CAnimationList::const_iterator citerend = m_AnimationList.end();
	while( citer != citerend )
	{
		(*citer)->Update( fDeltaTime);
		++citer;
	}

	// ÿ�������ͼ�Ķ���ĸ����߼�
	CXObjectList& objlist = CXObjectList::GetInstance();
	EnumChildParam param = { this, fDeltaTime };
	EnumChild( OnEnumChild, (LPVOID)&param );
	return TRUE;
}

bool CMap::OnEnumChild( int nID, CXObject* pObj, LPVOID pParam )
{
	if( pObj && pObj->IsType( TypeMapobj ) )
	{
		CGameObject* pMapObj = static_cast< CGameObject* >( pObj );
		EnumChildParam* param = (EnumChildParam*)pParam;
		pMapObj->Update( *param->pMap, param->fDeltaTime );
		if( pMapObj->IsType( TypeDynamic ) )
		{
			CDynamic* pMan = static_cast< CDynamic* >( pMapObj );
			CXVariant IsRun;
			pMan->GetLocalAttrib( Dynamic_attrMoveState, IsRun );
			if( IsRun )
			{
				param->pMap->MoveObject( *pMan, pMan->GetDirection(), pMan->GetSpeed()*param->fDeltaTime );
			}
		}
	}
	return true;
}

bool CMap::OnAddChild(int nID )
{
	PutObjToMap( nID );
	return __super::OnAddChild( nID );
}

bool CMap::OnRemoveChild( int nID, bool bRelease )
{
	RemoveObjFromMap( nID );
	return __super::OnRemoveChild( nID, bRelease );
}

// �õ���ǰ���������ڵ����и���
// rc		:	��������
// retQueue	:	���صĸ��Ӷ���

BOOL	CMap::GetCollisionBlock( const RECT& rc, CBlockList& retList )const
{
	int by = rc.top/GetBlockHeight();
	int bx = rc.left/GetBlockWidth();

	int ey = rc.bottom/GetBlockHeight();
	int ex = rc.right/GetBlockWidth();
	for( int y = by; y <= ey; y++ )
	{
		for( int x = bx; x <= ex; x++ )
		{
			retList.push_back( GetBlock( x, y ) );
		}
	}
	return TRUE;
}

// �����ײ
// return	:	�Ƿ�����ײ
// pBlock	:	���Ǹ����Ӽ����ײ
// CollisionRect	:	�Ǹ���������ײ
BOOL	CMap::TestCollision( CMapBlock* pBlock, const x_rect& CollisionRect, int nObjID )
{
	if( !pBlock )	return FALSE;
	x_rect rc;
	CMapBlock::CObjList::iterator iter = pBlock->m_ObjList.begin();
	CXObjectList& ObjList = CXObjectList::GetInstance();
	while( iter != pBlock->m_ObjList.end() )
	{
		int nID = *iter++;
		CGameObject* pObj = static_cast< CGameObject* >( ObjList.GetObj( nID ) );
		ASSERT_MSG( pObj, _T("��Ч��ָ�������") );
		if( pObj && nObjID != nID )
		{
			x_rect& rc2 = x_rect( pObj->GetCollision() );
			if( rc.IntersectRect( rc2, CollisionRect ) )
			{
				//TRACE( "��⵽����[%d,%d]����ײ\n", pBlock->m_ptPos.x, pBlock->m_ptPos.y );
				if( pObj->Think( nObjID, event_beCollision, NULL, 0 ) )
				{
					return TRUE;
				}

				CGameObject* pDynamicObj = static_cast< CDynamic* >( ObjList.GetObj( nObjID ) );
				ASSERT_MSG( pDynamicObj->IsType( TypeDynamic ), _T("����ȷ�Ķ�̬���ͣ�") );
				if( pDynamicObj->Think( *iter, event_Collision, NULL, 0 ) )
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//	Obj		:	���ƶ��Ķ���
//	enDir	:	�ƶ�����
//	nStep	:	�ƶ�����
//	return	:	�Ƿ��ƶ�
BOOL	CMap::MoveObject( CDynamic& Obj, float fDirection, float fSpeed )
{
	if( fSpeed == 0.0f )	return FALSE;

	float fPosX, fPosY, fPosZ;
	Obj.GetPos( fPosX, fPosY, fPosZ );

	float fNewPosX = fPosX;
	float fNewPosY = fPosY;
	float fNewPosZ = fPosZ;

	int nMapRightLeg	= (m_dwMapWidth - 1)*m_siBlockSize.cy;
	int nMapDownLeg		= (m_dwMapHeight - 1)*m_siBlockSize.cx;
	BOOL bRet = TRUE;
	const x_rect& ObjRect = Obj.GetCollision();
	// legRect �ܰ�Χ��ײ���ε���С�߽���Ρ�
	// �����������뷨������ı�����ײ�����԰�������С�߽�����еĻ���
	// ������Ҫά�����������б��������¼�����ӶԽ����ȶ�������á�
	x_rect legRect(
	ObjRect.left/m_siBlockSize.cx*m_siBlockSize.cx,	// �������
	ObjRect.top/m_siBlockSize.cy*m_siBlockSize.cy,	// �������
	( ObjRect.right + m_siBlockSize.cx )/m_siBlockSize.cx*m_siBlockSize.cx,
	( ObjRect.bottom + m_siBlockSize.cy )/m_siBlockSize.cy*m_siBlockSize.cy );

	float fSpeedX = fSpeed*sin( fDirection );
	if( fSpeedX > (float)m_siBlockSize.cx )
	{
		fSpeedX = (float)m_siBlockSize.cx;
	}
	fNewPosX = fPosX + fSpeedX;

	float fSpeedZ = fSpeed*cos( fDirection );
	if( fSpeedZ > m_siBlockSize.cy )
	{
		fSpeedZ = (float)m_siBlockSize.cy;
	}
	fNewPosZ = fPosZ + fSpeedZ;

	// ����ɫ��������Ϊ������
	//Obj.SetPos( fNewPosX, fNewPosY, fNewPosZ );

	x_rect NewObjRect( ObjRect );
	NewObjRect.OffsetRect( (int)fNewPosX - (int)fPosX, (int)fNewPosZ - (int)fPosZ );

	//if( bRet )
	{
		// �õ��µı߽�����������ĸ���
		CBlockList	retList;
		GetCollisionBlock( NewObjRect, retList ); 
		CBlockList::const_iterator citerList = retList.begin();
		while( citerList != retList.end() )
		{
			// ��ÿ��������Ķ��������ײ���
			CMapBlock* pBlock = *citerList;
			//TRACE( "Block[%d,%d] was detected!\n", pBlock->m_ptPos.x, pBlock->m_ptPos.y );
			if( pBlock && TestCollision( pBlock, NewObjRect, Obj.GetObjID() ) )
			{
				return FALSE;
			}
			++citerList;
		}

		// ����ϲ���ƶ��ж�ʧ���򷵻�
		if( !OnDynamicMove( Obj, fNewPosX, fNewPosY, fNewPosZ ) )	return FALSE;

		Obj.SetPos( fNewPosX, fNewPosY, fNewPosZ );
		const x_rect& NewObjRect = Obj.GetCollision();
		x_rect NewLegRect(
			NewObjRect.left/m_siBlockSize.cx*m_siBlockSize.cx,
			NewObjRect.top/m_siBlockSize.cy*m_siBlockSize.cy,
			( NewObjRect.right + m_siBlockSize.cx )/m_siBlockSize.cx*m_siBlockSize.cx,
			( NewObjRect.bottom + m_siBlockSize.cy )/m_siBlockSize.cy*m_siBlockSize.cy );

		if( !legRect.PtInRect( NewObjRect.TopLeft() ) ||
			!legRect.PtInRect( NewObjRect.BottomRight() ) || 
			NewObjRect != legRect )
		{
			// �µ���ײ���β��ٱ߽�����ڵ�ʱ��Ҫ���µ�����������
			Obj.SetPos( fPosX, fPosY, fPosZ );
			RemoveObjFromMap( Obj.GetObjID() );
			// RemoveChild( Obj.GetObjID() );
			Obj.SetPos( fNewPosX, fNewPosY, fNewPosZ );
			PutObjToMap( Obj.GetObjID() );
			// AddChild( Obj.GetObjID() );
		}
	}
	return bRet;
}

BOOL	CMap::PutObjToMap( int nObjID )			// ����������ͼ
{
	CXObject* pObj = CXObjectList::GetInstance().GetObj( nObjID );
	if( !pObj )	return FALSE;
	if( !pObj->IsType( TypeMapobj ) )	return FALSE;
	CGameObject* pGameObj = static_cast< CGameObject* >( pObj );
	x_rect rc = pGameObj->GetCollision();
	CBlockList	retList;
	GetCollisionBlock( rc, retList );

	//char str[1024];
	//int nLen = _snprintf( str, sizeof( str ), "��ǰռ�и���" );
	CBlockList::const_iterator citer = retList.begin();
	while( citer != retList.end() )
	{
		CMapBlock* pBlock = *citer;
		ASSERT_MSG( pBlock, _T("��Ч��ָ�������") );
		//nLen += _snprintf( str + nLen, sizeof( str ) - nLen, "[%d,%d]", pBlock->m_ptPos.x, pBlock->m_ptPos.y );
		pBlock->AddObject( nObjID );
		pGameObj->SetAttrib( TypeMapobj, Mapobj_attrScene, GetObjID() );
		++citer;
	}
	//_snprintf( str + nLen, sizeof( str ) - nLen, "\n" );
	//TRACE( str );
	// m_ObjMap[nObjID] = pGameObj;
	//pObj->SetParent( GetObjID() );
	//pObj->SetAttrib( TypeMapobj, Mapobj_attrScene, GetObjID() );
	//AddChild( nObjID );
	return TRUE;
}

BOOL	CMap::RemoveObjFromMap( int nObjID )	// ������ӵ�ͼ�Ƴ�
{
	CGameObject* pObj = ( CGameObject* )CXObjectList::GetInstance().GetObj( nObjID );
	if( !pObj )	return FALSE;
	x_rect rc = pObj->GetCollision();
	CBlockList	retList;
	GetCollisionBlock( rc, retList );
	CBlockList::const_iterator citer = retList.begin();
	while( citer != retList.end() )
	{
		CMapBlock* pBlock = *citer;
		ASSERT_MSG( pBlock, _T("��Ч��ָ�������") );
		pBlock->DelObject( nObjID );
		pObj->SetAttrib( TypeMapobj, Mapobj_attrScene, -1 );
		++citer;
	}
	// m_ObjMap.erase( nObjID );
	//pObj->SetParent( INVALID_OBJID );
	//pObj->SetAttrib( TypeMapobj, Mapobj_attrScene, INVALID_OBJID );
	//RemoveChild( nObjID );
	return TRUE;
}
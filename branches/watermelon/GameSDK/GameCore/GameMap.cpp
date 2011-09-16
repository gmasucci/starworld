#include "StdAfx.h"
#include "GameObject.h"
#include "GameMap.h"
namespace XGC
{
	//////////////////////////////////////////////////////////////////////////
	// CGameMap
	// ��ʼ����ͼ
	extern timer& ThisTimer();
	CServerMap::CServerMap()
		: m_nCellCountW( 0 )		// ��ש�������
		, m_nCellCountH( 0 )	// ��ש�������
		, m_InvBW( 0.0f )
		, m_InvBH( 0.0f )
		, m_fTransfromX( 0 )
		, m_fTransfromY( 0 )
		, m_fCellWidth( 0.0f )
		, m_fCellHeight( 0.0f )
		, m_pCells( NULL )
		, m_pCell( NULL )
		, m_fAreaWidth( 0.0f )
		, m_fAreaHeight( 0.0f )
		, m_nAreaCountW( 0 )
		, m_nAreaCountH( 0 )
		, m_pAreas( NULL )
		DEBUG_CODE( , m_bChangeBlock( false ) )
	{
		// ��ͼ������Ϣ
	}

	void CServerMap::InitializeMap( int nCellCountW, int nCellCountH, int nAreaWidthOfCell, int nAreaHeightOfCell, float fCellWidth, float fCellHeight, float fTransfromX, float fTransfromY )
	{
		m_pCells = new MapCell[nCellCountW*nCellCountH];
		m_pCell = new MapCell*[nCellCountH];
		MapCell* pCell = m_pCells;
		for( int i = 0; i < nCellCountH; ++i, pCell += nCellCountW )
		{
			m_pCell[i] = pCell;
		}


		m_nCellCountW	= nCellCountW;
		m_nCellCountH	= nCellCountH;
		m_fCellWidth	= fCellWidth;
		m_fCellHeight	= fCellHeight;
		m_fTransfromX	= fTransfromX;
		m_fTransfromY	= fTransfromY;

		m_fAreaWidth	= nAreaWidthOfCell*fCellWidth;
		m_fAreaHeight	= nAreaHeightOfCell*fCellHeight;
		m_nAreaWidthOfCell	= nAreaWidthOfCell;
		m_nAreaHeightOfCell = nAreaHeightOfCell;
		m_nAreaCountW = (nCellCountW + nAreaWidthOfCell - 1)/nAreaWidthOfCell;
		m_nAreaCountH = (nCellCountH + nAreaHeightOfCell - 1)/nAreaHeightOfCell;

		m_pAreas = new AreaSet[m_nAreaCountW*m_nAreaCountH];

		m_InvBW = 1.0f/fCellWidth;
		m_InvBH = 1.0f/fCellHeight;
	}

	void CServerMap::DestroyMap()
	{
		m_nCellCountW	= 0;
		m_nCellCountH	= 0;
		m_fCellWidth	= 0.0f;
		m_fCellHeight	= 0.0f;
		m_fTransfromX	= 0.0f;
		m_fTransfromY	= 0.0f;

		m_fAreaWidth	= 0.0f;
		m_fAreaHeight	= 0.0f;
		m_nAreaWidthOfCell	= 0;
		m_nAreaHeightOfCell = 0;
		m_nAreaCountW = 0;
		m_nAreaCountH = 0;

		m_InvBW = 0.0f;
		m_InvBH = 0.0f;

		SAFE_DELETE_ARRAY( m_pCell );
		SAFE_DELETE_ARRAY( m_pCells );

		for( ReciverMap::iterator i = m_ReciverMap.begin(); i != m_ReciverMap.end(); ++i )
		{
			i->second->Destroy();
		}
		m_ReciverMap.clear();
	}

	//---------------------------------------------------//
	// [1/5/2011 Albert]
	// Description:	�ڵ�ͼ����ӳ������� 
	//---------------------------------------------------//
	bool CServerMap::Insert( CGameObject* pObject, const XVector3& vPosition )
	{
		pObject->SetPosition( vPosition );

		if( !pObject->PreEnterMap( this ) )
			return false;

		if( !AddChild( pObject ) )
			return false;

		if( !ExchangeBlock( pObject, iPoint(-1,-1), WorldToArea( vPosition.x, vPosition.z ), iSize(1,1) ) )
			return false;

		pObject->OnEnterMap( this );
		return true;
	}

	//---------------------------------------------------//
	// [1/5/2011 Albert]
	// Description:	�ӳ������Ƴ����� 
	//---------------------------------------------------//
	void CServerMap::Remove( CGameObject* pObject )
	{
		if( !pObject->PreLeaveMap( this ) )
			return;

		__super::RemoveChild( pObject );
		XVector3 vPosition = pObject->GetPosition();
		ExchangeBlock( pObject, WorldToArea( vPosition.x, vPosition.z ), iPoint( -1, -1 ), iSize(1,1) );

		pObject->OnLeaveMap( this );
	}

	void CServerMap::CreateSpoorer( _lpctstr lpszSpoorerName, float fSecond )
	{
		ThisTimer().insert_event( Spoorer( lpszSpoorerName, GetObjectID() ), 1, -1, TIMER_SECONDS(fSecond) );
	}

	void CServerMap::InsertTrigger( const _string& strSourceName, int nTriggerType, int nTriggerCode, const _string& strReciverName )
	{
		Trigger t;
		t.nTriggerType = nTriggerType;
		t.nTriggerCode = nTriggerCode;
		t.strReciverName = strReciverName;

		m_TriggerMap.insert( TriggerMap::value_type( strSourceName, t ) );
	}

	//---------------------------------------------------//
	// [1/5/2011 Albert]
	// Description:	���������� 
	//---------------------------------------------------//
	void CServerMap::BuildTrigger( const _string& strSourceName, CGameObject* pServerObject )
	{
		pServerObject->SetStrAttrib( _T("Alias"), strSourceName.c_str() );
		TriggerMap::_Pairii tRange = m_TriggerMap.equal_range( strSourceName );
		for( TriggerMap::iterator t = tRange.first; t != tRange.second; ++t )
		{
			// ����Դ�����ֲ��Ҵ����¼�
			ReciverMap::_Pairii i = m_ReciverMap.equal_range( t->second.strReciverName );
			for( ReciverMap::iterator r = i.first; r != i.second; ++r )
			{
				// ���ҽ��ն�����
				pServerObject->RegisteTrigger( t->second.nTriggerType, bind( &CServerMap::TriggerReciver, this, _1, _2, _3, _4, t->second.nTriggerCode, r->second ) );
			}
		}
	}


	bool CServerMap::TestCollisionBlock( int x, int y )
	{
		ASSERT( x < m_nCellCountW && y < m_nCellCountH );
		return m_pCell[x][y].barrier == 0;
	}

	// �����ײ
	// return	:	�Ƿ�����ײ
	// pArea	:	���Ǹ����Ӽ����ײ
	// CollisionRect	:	�Ǹ���������ײ
	bool CServerMap::TestCollision( int x0, int y0, int &x1, int &y1 )
	{
		int x = x0, y = y0;
		if( x0 != x1 || y0 != y1 )
		{
			int sx = ((x1-x0)>0)?1:-1;
			int sy = ((y1-y0)>0)?1:-1;

			int dx = abs( x1 - x0 );
			int dy = abs( y1 - y0 );

			bool steep = dy > dx;
			if( steep )
			{
				std::swap(x,y);
				std::swap(dx,dy);
				std::swap(sx,sy);

				int e = (dy << 1) - dx;

				for( int i = 0; i < dx; ++i )
				{
					int tx = x, ty = y;
					while( e >= 0 )
					{
						if( TestCollisionBlock( y, x + sx ) && TestCollisionBlock( y + sy, x ) )
						{
							x1 = ty;
							y1 = tx;
							return true;
						}

						y += sy;
						e -= (dx << 1);
					}
					x += sx;
					e += (dy << 1);

					// collision test
					if( TestCollisionBlock( y, x ) )
					{
						// ��x,y��Ϊ���һ�����ߵ�Cell������
						x1 = ty;
						y1 = tx;
						return true;
					}
				}
			}
			else
			{
				int e = (dy << 1) - dx;

				for( int i = 0; i < dx; ++i )
				{
					int tx = x, ty = y;
					while( e >= 0 )
					{
						if( TestCollisionBlock( x, y + sy ) && TestCollisionBlock( x + sx, y ) )
						{
							x1 = tx;
							y1 = ty;
							return true;
						}

						y += sy;
						e -= (dx << 1);
					}
					x += sx;
					e += (dy << 1);

					// collision test
					if( TestCollisionBlock( x, y ) )
					{
						// ��x,y��Ϊ���һ�����ߵ�Cell������
						x1 = tx;
						y1 = ty;
						return true;
					}
				}
			}
		}
		return false;
	}

	//	Obj		:	���ƶ��Ķ���
	//	enDir	:	�ƶ�����
	//	nStep	:	�ƶ�����
	//	return	:	�Ƿ��ƶ�
	bool CServerMap::DynamicMoveTo( CGameObject* pObject, XVector3 &vPositionNew, bool bCollisionTest )
	{
		bool ret = true;

		XVector3 vPositionOld = pObject->GetPosition();
		if( !pObject->GetObjectFlag( CGameObject::Flag_Move ) )
		{
			vPositionNew = vPositionOld;
			return false;
		}

		if( bCollisionTest )
		{
			iPoint iOldCell = WorldToCell( vPositionOld.x, vPositionOld.z );
			iPoint iNewCell = WorldToCell( vPositionNew.x, vPositionNew.z );

			if( TestCollision( iOldCell.x, iOldCell.y, iNewCell.x, iNewCell.y ) )
			{
				// ����Ϊ����ײ���ӵ����ĵ�
				fPoint fPosition = CellToWorld( iNewCell.x, iNewCell.y );
				vPositionNew.x = fPosition.x;
				vPositionNew.y = 0.0f;
				vPositionNew.z = fPosition.y;

				ret = false;
			}
		}


		iPoint iOldArea = WorldToArea( vPositionOld.x, vPositionOld.z );
		iPoint iNewArea = WorldToArea( vPositionNew.x, vPositionNew.z );

		//DEBUG_CODE( CMapBlock *pArea = GetBlock(x,y); if( pArea ) ASSERT_MSG( pArea->CheckExist(pObj->GetObjID()), _T("���ַɻ���") ); )
		if( ExchangeBlock( pObject, iOldArea, iNewArea, iSize(1,1) ) )
		{
			pObject->SetPosition( vPositionNew );
		}
		return ret;
	}

	bool CServerMap::ExchangeBlock( CGameObject* pObject, const iPoint& iOldArea, const iPoint& iNewArea, const iSize& iEyeshot )
	{
		DEBUG_CODE( ASSERT( m_bChangeBlock == false ) );
		DEBUG_CODE( m_bChangeBlock = true );

		if( iOldArea == iNewArea )
		{
			DEBUG_CODE( m_bChangeBlock = false );
			return true;
		}

		std::list< xObject > leave_eyeshot_list;
		std::list< xObject > enter_eyeshot_list;

		// �滻�ɸ��ӣ�����¸���
		for( int x = -iEyeshot.cx; x <= iEyeshot.cx; ++x )
		{
			for( int y = -iEyeshot.cy; y <= iEyeshot.cy; ++y )
			{

				if( abs(iOldArea.x + x - iNewArea.x) <= iEyeshot.cx &&
					abs(iOldArea.y + y - iNewArea.y) <= iEyeshot.cy)
				{
					continue;
				}

				AreaSet* pOldArea = GetArea( iOldArea.x + x, iOldArea.y + y );
				AreaSet* pNewArea = GetArea( iNewArea.x - x, iNewArea.y - y );

				if( pOldArea )
				{
					// ֪ͨ�뿪��Ұ
					copy( pOldArea->begin(), pOldArea->end(), back_inserter(leave_eyeshot_list) );
				}

				if( pNewArea )
				{
					// ֪ͨ������Ұ
					copy( pNewArea->begin(), pNewArea->end(), back_inserter(enter_eyeshot_list) );
				}
			}
		}

		// �任��������
		// ȡ���ɵĸ�������
		AreaSet* pOldArea = GetArea( iOldArea.x, iOldArea.y );
		AreaSet* pNewArea = GetArea( iNewArea.x, iNewArea.y );

		if( pOldArea )
		{
			pOldArea->erase( pObject->AreaIterator() );
		}

		if( pNewArea )
		{
			AreaSet::_Pairib ib = pNewArea->insert( pObject->GetObjectID() );
			ASSERT( ib.second );
			pObject->AreaIterator( ib.first );
		}
		DEBUG_CODE( m_bChangeBlock = false );

		// ������Ұ
		if( !leave_eyeshot_list.empty() )
			for_each( leave_eyeshot_list.begin(), leave_eyeshot_list.end(), bind( &CServerMap::NotifyLeaveEyeshot, this, pObject, _1 ) );
		// �뿪��Ұ
		if( !enter_eyeshot_list.empty() )
			for_each( enter_eyeshot_list.begin(), enter_eyeshot_list.end(), bind( &CServerMap::NotifyEnterEyeshot, this, pObject, _1 ) );

		return true;
	}

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	֪ͨ�����ڵ����ж���,���˽�������Ұ
	// nObjID:		��֪ͨ�Ķ���ID
	// pObj:		�����ƶ��Ķ���
	//---------------------------------------------------//
	void CServerMap::NotifyEnterEyeshot( CGameObject* pObj, xObject nObjID )
	{
		CGameObject* pTarget = static_cast< CGameObject* >( ObjectPool.GetObject( nObjID, TypeServerObject ) );
		if( !pTarget || !pObj || pTarget == pObj )
			return;

		// XGC_WRITELOG( LOG, _T("%08x�ƶ�����%08x����Ұ��"), pObj->GetObjID(), nObjID );
		pTarget->EnterEyeshot( pObj );
		pObj->EnterEyeshot( pTarget );
	}

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	֪ͨ�����ڵ����ж���,�����뿪����Ұ
	// nObjID:		��֪ͨ�Ķ���ID
	// pObj:		�����ƶ��Ķ���
	//---------------------------------------------------//
	void CServerMap::NotifyLeaveEyeshot( CGameObject* pObj, xObject nObjID )
	{
		CGameObject* pTarget = static_cast< CGameObject* >( ObjectPool.GetObject( nObjID, TypeServerObject ) );
		if( !pTarget || !pObj || pTarget == pObj )
			return;

		// XGC_WRITELOG( LOG, _T("%08x�ƶ��뿪%08x����Ұ��"), pObj->GetObjID(), nObjID );
		pTarget->LeaveEyeshot( pObj );
		pObj->LeaveEyeshot( pTarget );
	}

	//---------------------------------------------------//
	// [12/29/2010 Albert]
	// Description:	 �������������ȡ����
	//---------------------------------------------------//
	inline CServerMap::AreaSet* CServerMap::GetArea( float x, float y )const
	{
		int x0 = (int)( (x - m_fTransfromX) / m_fAreaWidth );
		int y0 = (int)( (y - m_fTransfromY) / m_fAreaHeight );

		return GetArea( x0, y0 );
	}

	inline CServerMap::AreaSet* CServerMap::GetArea( int x, int y )const
	{
		if( x < 0 || x >= m_nAreaCountW || y < 0 || y >= m_nAreaCountH )	return xgc_nullptr;

		return m_pAreas?m_pAreas + (y*m_nAreaCountW+x):xgc_nullptr;
	}

	//int CServerMapArea::GetAreaList( int x0, int y0, int x1, int y1, CAreaList& ret )const
	//{
	//	for( int y = y0; y <= y1; y++ )
	//	{
	//		for( int x = x0; x <= x1; x++ )
	//		{
	//			AreaSet *pArea = GetArea( x, y );
	//			if( pArea )
	//			{
	//				ret.push_back( pArea );
	//			}
	//		}
	//	}
	//	return ret.size();
	//}

	//int CServerMapArea::GetAreaList( iPoint ptBegin, iPoint ptEnd, CAreaList& ret )const
	//{
	//	return GetAreaList( ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y, ret );
	//};

	//int CServerMapArea::GetAreaList( iRect rcBlockArea, CAreaList& ret )const
	//{
	//	return GetAreaList( rcBlockArea.left, rcBlockArea.top, rcBlockArea.right, rcBlockArea.bottom, ret );
	//};

	////---------------------------------------------------//
	//// [9/3/2009 Albert]
	//// Description:	��ȡ�����ڵ����ж���ID
	////---------------------------------------------------//
	//int	CServerMapArea::GetTargets( const CAreaList& bl, AreaSet& ret )
	//{
	//	struct Operator
	//	{
	//		AreaSet& _set;

	//		Operator( AreaSet& set )
	//			: _set( set )
	//		{

	//		}

	//		void operator()( xObject id ){ _set.insert( id ); }
	//	};

	//	CAreaList::const_iterator ci = bl.begin();
	//	while( ci != bl.end() )
	//	{
	//		AreaSet *pArea = *ci;

	//		if( pArea )
	//		{
	//			for_each( pArea->begin(), pArea->end(),  Operator( ret ) );
	//		}
	//		++ci;
	//	}
	//	return ret.size();
	//}

	////---------------------------------------------------//
	//// [11/29/2010 Albert]
	//// Description:	��ȡһ����Χ�ڵ�����
	//// return ��ȡ�Ķ������
	////---------------------------------------------------//
	//int CServerMapArea::GetRangeTarget( const XVector3& vCenter, float fRangeMin, float fRangeMax, xObject hObject[], size_t nCount, bool (*Fillter)( xObject hObject, long_ptr lParam ), long_ptr lParam )
	//{
	//	int x = (int)( (vCenter[0] - m_fTransfromX) / (m_nAreaWidthOfCell * m_fCellWidth) );
	//	int y = (int)( (vCenter[2] - m_fTransfromY) / (m_nAreaHeightOfCell * m_fCellHeight) );

	//	// ��ȡ�����ŵķ�Χ
	//	int nRadiusX = int(fRangeMax/( m_nAreaWidthOfCell * m_fCellWidth ))+1;
	//	int nRadiusY = int(fRangeMax/( m_nAreaHeightOfCell * m_fCellHeight))+1;

	//	CAreaList areas;
	//	GetAreaList( x - nRadiusX, y - nRadiusY, x + nRadiusX, y + nRadiusY, areas );

	//	AreaSet targets;
	//	GetTargets( areas, targets );

	//	int n = 0;
	//	for( AreaSet::iterator j = targets.begin(); j != targets.end() && n != nCount; ++j )
	//	{
	//		CServerObject* pObject = static_cast< CServerObject* >( ObjectPool.GetObject( *j, TypeServerObject ) );
	//		if( pObject )
	//		{
	//			XVector3 vDistance = pObject->GetPosition() - vCenter;
	//			float fDistance = vDistance.Length();
	//			if( fDistance > fRangeMin && fDistance <= fRangeMax )
	//			{
	//				if( (Fillter && Fillter( *j, lParam )) || Fillter == xgc_nullptr )
	//					hObject[n++] = *j;
	//			}
	//		}
	//	}

	//	return n;
	//}

}

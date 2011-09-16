#include "StdAfx.h"
#include "ObjectControlerBase.h"
#include "GameObject.h"
#include "GameMap.h"

namespace XGC
{
	extern timer& ThisTimer();
	CObjectControlerBase::CObjectControlerBase()
		: m_pHost( xgc_nullptr )
		, m_hUpdateTimer( INVALID_TIMER_HANDLE )
	{
	}

	CObjectControlerBase::CObjectControlerBase( CGameObject *pHost )
		: m_pHost( pHost )
		, m_hUpdateTimer( INVALID_TIMER_HANDLE )
	{
		Attach( pHost );
	}

	CObjectControlerBase::~CObjectControlerBase(void)
	{
		StopUpdate();
		if( m_pHost )
		{
			m_pHost->ResetControler( xgc_nullptr );
			m_pHost = NULL;
		}
	}

	//---------------------------------------------------//
	// [11/24/2010 Albert]
	// Description:	���ӿ��ƿ��ƶ� 
	//---------------------------------------------------//
	void CObjectControlerBase::Attach( CGameObject* pObject )
	{
		if( pObject )
		{
			pObject->ResetControler( this );
			m_pHost = pObject;
		}
	}

	//---------------------------------------------------//
	// [11/24/2010 Albert]
	// Description:	�Ͽ����ƶ�
	//---------------------------------------------------//
	CGameObject* CObjectControlerBase::Detach()
	{
		CGameObject* pObject = m_pHost;
		if( m_pHost )
		{
			StopUpdate();
			pObject->ResetControler( xgc_nullptr );
			m_pHost = xgc_nullptr;
		}

		return pObject;
	}

	void CObjectControlerBase::SetUpdate( const _tfunction &fn, int nRepeat, int nInterval, int nDelay )
	{
		ASSERT( m_hUpdateTimer == INVALID_TIMER_HANDLE );
		m_hUpdateTimer = ThisTimer().insert_event( fn, nRepeat, nInterval, nDelay );
	}

	//---------------------------------------------------//
	// [12/1/2010 Albert]
	// Description:	��ͣ�����߼� 
	//---------------------------------------------------//
	void CObjectControlerBase::PauseUpdate()
	{
		ThisTimer().pause_event( m_hUpdateTimer );
	}

	//---------------------------------------------------//
	// [12/1/2010 Albert]
	// Description:	�ָ������߼� 
	//---------------------------------------------------//
	void CObjectControlerBase::ResumeUpdate()
	{
		ThisTimer().resume_event( m_hUpdateTimer );
	}

	//---------------------------------------------------//
	// [9/16/2009 Albert]
	// Description:	ֹͣ����
	//---------------------------------------------------//
	unsigned int CObjectControlerBase::StopUpdate()
	{
		unsigned int nRelease = 0;
		if( m_hUpdateTimer != INVALID_TIMER_HANDLE )
		{
			nRelease = ThisTimer().remove_event( m_hUpdateTimer );
			m_hUpdateTimer = INVALID_TIMER_HANDLE;

			OnStopUpdate( nRelease );
		}

		return nRelease;
	}

	//////////////////////////////////////////////////////////////////////////
	//---------------------------------------------------//
	// [8/4/2009 Albert]
	// Description:	���·����
	//---------------------------------------------------//
	CObjectMoveControler::CObjectMoveControler( CGameObject* pObject, bool bCollision )
		: CObjectControlerBase( pObject )
		, m_bCollision( bCollision )
	{

	}

	CObjectMoveControler::~CObjectMoveControler()
	{
	}

	void CObjectMoveControler::OnStopUpdate( unsigned int nReleaseTime )
	{
		if( nReleaseTime != -1 )
		{
			// ��������
			XVector3 Vec = m_vTargetPosition - m_vNextPosition;
			Vec.NormalizeFast();
			// ʣ���˶��پ���
			Vec *= nReleaseTime*TIMER_INTERVAL/1000.0f;
			// �´˸��������ʣ�����Ĳ�ֵ��Ϊ��ǰ��λ��
			m_vNextPosition -= Vec;

			//CServerMap* pMap = static_cast< CServerMap* >( ObjectPool.GetObject( m_pHost->GetParent(), TypeMap ) );
			//if( pMap )
			//{
			//	ASSERT( m_pHost );
			//	pMap->DynamicMoveTo( static_cast< CServerObject* >( m_pHost ), m_vNextPosition, m_bCollision );
			//	ASSERT( m_pHost );
			//}
		}
	}

	//---------------------------------------------------//
	// [8/4/2009 Albert]
	// Description:	���·��
	//---------------------------------------------------//
	void CObjectMoveControler::MoveTo( XVector3 vPosition, float fSpeed, float fDelay )
	{
		m_vNextPosition = m_pHost->GetPosition();
		m_vTargetPosition = vPosition;
		m_vSpeed = m_vTargetPosition - m_vNextPosition;
		m_vSpeed.NormalizeFast();
		m_vSpeed *= fSpeed*0.25f;
		m_fSpeed = fSpeed;

		m_pHost->Trigger( TypeServerObject, INVALID_OBJECT_ID, (long_ptr)this, CGameObject::Event_MoveBegin );
		// timer ����Ϊ -1 ���ʾ������ִ��
		if( !IsUpdate() )
		{
			SetUpdate(
				bind( &CObjectMoveControler::UpdatePosition, this, _1 ),
				-1, 
				TIMER_SECONDS( 0.25f ), 
				TIMER_SECONDS( CalcNextPosition() ) );
		}
	}

	//---------------------------------------------------//
	// [8/5/2009 Albert]
	// Description:	�ƶ���ʱ��
	//---------------------------------------------------//
	bool CObjectMoveControler::UpdatePosition( unsigned int handle )
	{
		ASSERT_RETURN( m_pHost, true );
		// ���ƶ�����ǰ���ڵ�λ��.
		CServerMap* pMap = static_cast< CServerMap* >( ObjectPool.GetObject( m_pHost->GetParent(), TypeMap ) );
		if( pMap )
		{
			if( pMap->DynamicMoveTo( static_cast< CGameObject* >( m_pHost ), m_vNextPosition, m_bCollision ) )
			{
				// λ�ø��¿��ܵ���AI�뿪MoveState, �Ӷ����������Ͽ�������������ӡ�
				if( m_pHost == xgc_nullptr )
					return true;

				//CLogger::GetInstance(_T("Log"))->WriteLog( _T("��ɫ[%08x]�ƶ���·��[%f,%f,%f]")
				//	, GetObjID()
				//	, m_vTargetPosition[0], m_vTargetPosition[1], m_vNextPosition[2] );
				if( m_vTargetPosition == m_vNextPosition )
				{
					m_pHost->Trigger( TypeServerObject, INVALID_OBJECT_ID, (long_ptr)this, CGameObject::Event_MoveArrived ); // Arrived 
				}
				else
				{
					m_pHost->Trigger( TypeServerObject, INVALID_OBJECT_ID, (long_ptr)this, CGameObject::Event_MoveStep ); // Step 
					_tevent* event = ThisTimer().get_event( handle );
					event->set_interval( TIMER_SECONDS(CalcNextPosition()) );
					return false;
				}
			}
			else if( m_pHost )
			{
				m_pHost->Trigger( TypeServerObject, INVALID_OBJECT_ID, (long_ptr)this, CGameObject::Event_MoveStop ); // Stop 
			}
		}
		StopUpdate();
		return true;
	}

	float CObjectMoveControler::CalcNextPosition()
	{
		// ������һ�����µ�
		XVector3 vDistance = m_vTargetPosition - m_vNextPosition;

		if( vDistance.SqurLength() < m_vSpeed.SqurLength() )
		{
			m_vNextPosition = m_vTargetPosition;
		}
		else
		{
			m_vNextPosition += m_vSpeed;
		}

		return 0.25f;
	}


	//////////////////////////////////////////////////////////////////////////
	CObjectIdleControler::CObjectIdleControler( CGameObject* pObject )
		: CObjectControlerBase( pObject )
		, m_fIdleTime( 0.0f )
	{

	}

	CObjectIdleControler::~CObjectIdleControler()
	{
	}

	void CObjectIdleControler::Idle( float fTime /*= 0.0f */ )
	{
		SetUpdate( 
			bind( &CObjectIdleControler::UpdateIdle, this, _1 ), 
			1, 
			TIMER_SECONDS(fTime), 
			0 );
	}

	bool CObjectIdleControler::UpdateIdle( unsigned int handle )
	{
		m_pHost->Trigger( TypeServerObject, INVALID_OBJECT_ID, (long_ptr)this, CGameObject::Event_Stand );
		StopUpdate();
		return true;
	}

}

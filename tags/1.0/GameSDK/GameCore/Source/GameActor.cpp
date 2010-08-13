#include "StdAfx.h"
#include "GameActor.h"
#include "GameActorAI.h"

CGameActor::CGameActor(void)
: m_pAI( NULL )
, m_hEyeshotGroup( -1 )
, m_ActorStatus( status_live )
{
}

CGameActor::~CGameActor(void)
{
	SAFE_DELETE( m_pAI );
}

bool CGameActor::Think( int nObjID, int nEvent, long_ptr lParam )
{
	return m_pAI?m_pAI->Think( nObjID, nEvent, lParam ):false;
}

//---------------------------------------------------//
// [8/3/2009 Albert]
// Description:	���������Ұ
// pObject	:	������Ұ�Ķ���ָ��
//---------------------------------------------------//
void CGameActor::EnterEyeshot( CDynamicObject* pObject )
{
	if( m_pAI )
	{
		m_pAI->Think( pObject->GetObjID(), CGameActorAI::event_actor_enter_eyeshot, 0 );
	}
}

//---------------------------------------------------//
// [8/3/2009 Albert]
// Description:	�����뿪��Ұ
// pObject	:	�뿪��Ұ�Ķ���ָ��
//---------------------------------------------------//
void CGameActor::LeaveEyeshot( CDynamicObject* pObject )
{
	if( m_pAI )
	{
		m_pAI->Think( pObject->GetObjID(), CGameActorAI::event_actor_leave_eyeshot, 0 );
	}
}

//---------------------------------------------------//
// [9/9/2009 Albert]
// Description:	����Ŀ��ʱ����
//---------------------------------------------------//
void CGameActor::OnArrived( const XVector3& vPosition )
{
	if( m_pAI )
	{
		m_pAI->Think( INVALID_OBJID, CGameActorAI::event_actor_arrived, (long_ptr)&vPosition );
	}
}

//---------------------------------------------------//
// [9/10/2009 Albert]
// Description:	ÿ���ƶ������
//---------------------------------------------------//
void CGameActor::OnStep( const XVector3& vPosition )
{
	if( m_pAI )
	{
		m_pAI->Think( INVALID_OBJID, CGameActorAI::event_actor_step, (long_ptr)&vPosition );
	}
}

//---------------------------------------------------//
// [8/21/2009 Albert]
// Description:	��ɫ����
//---------------------------------------------------//
void CGameActor::Dead( _uint32 hAttacker )
{
	m_ActorStatus = status_dead;
	OnDead( hAttacker );
}

void CGameActor::OnDead( _uint32 hAttacker )
{
	if( m_pAI )
	{
		m_pAI->Think( hAttacker, CGameActorAI::event_actor_dead, 0 );
	}
}
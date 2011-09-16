#include "StdAfx.h"
#include "XAction.h"
#include "XSkillBase.h"
#include "XActorBase.h"
//////////////////////////////////////////////////////////////////////////
namespace XGC
{
	BEGIN_IMPLEMENT_ATTRIBUTE( XSkillBase, CXObject )
		MAP_ATTRIBUTE( attrConsume )
		MAP_ATTRIBUTE( attrCastingTime )
		MAP_ATTRIBUTE( attrCooldownTime )
		MAP_ATTRIBUTE( attrDamageMin )
		MAP_ATTRIBUTE( attrDamageMax )
		MAP_ATTRIBUTE( attrAttackRangeMin )
		MAP_ATTRIBUTE( attrAttackRangeMax )
	END_IMPLEMENT_ATTRIBUTE()

	extern timer& ThisTimer();
	XSkillBase::XSkillBase()
		: CXObject( true, false, false )
		, m_hCastingTimerHandler( INVALID_TIMER_HANDLE )
		, m_hCooldownTimerHandler( INVALID_TIMER_HANDLE )
		, m_hLifetimeTimerHandler( INVALID_TIMER_HANDLE )
	{
	}

	XSkillBase::~XSkillBase(void)
	{
		if( IsCasting() )
			ClearCasting();

		if( IsCooldown() )
			ClearCooldown();

		if( !IsOver() )
			ClearLifetime();
	}

	bool XSkillBase::PreAddChild( CXObject* pChild )
	{
		ASSERT_RETURN( pChild->IsTypeOf( TypeAction ), false );

		return true;
	}

	//---------------------------------------------------//
	// [12/29/2010 Albert]
	// Description:	��ʼʹ�ü��� 
	//---------------------------------------------------//
	void XSkillBase::CastSkill( xObject hTarget )
	{
		XActorBase* pActor = ObjectPool.GetObject< XActorBase >( GetParent() );
		if( pActor )
		{
			pActor->Trigger( TypeSkillBase, GetObjectID(), hTarget, SkillBegin );
		}
	}

	int XSkillBase::Attack( xObject hTarget )
	{
		int uFlags = 0;
		XActorBase* pActor = ObjectPool.GetObject< XActorBase >( GetParent() );
		if( pActor )
		{
			SET_LOCAL_VALUE( XActorBase, pActor, attrDamage, 0 );
			CChildList::const_iterator citer = begin();
			while( citer != end() )
			{
				CXAction* pAction = ObjectPool.GetObject< CXAction >( *citer );
				if( pAction )
				{
					uFlags |= pAction->ActiveAction( GetObjectID(), hTarget );
				}
				++citer;
			}
		}

		return uFlags;
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	���ü����ͷ�״̬ 
	//---------------------------------------------------//
	void XSkillBase::MarkCasting( float fDelay )
	{
		ASSERT_MSG( m_hCastingTimerHandler == INVALID_TIMER_HANDLE, _T("�����Ѿ����ͷŹ����С�") );
		m_hCastingTimerHandler = ThisTimer().insert_event( bind( &XSkillBase::ResetCasting, this, _1 ), -1, -1, TIMER_SECONDS(fDelay) );
	}

	void XSkillBase::ClearCasting( bool bTrigger )
	{
		ThisTimer().remove_event( m_hCastingTimerHandler );
		m_hCastingTimerHandler = INVALID_TIMER_HANDLE;

		if( bTrigger )
		{
			XActorBase* pActor = ObjectPool.GetObject< XActorBase >( GetParent() );
			if( pActor )
			{
				pActor->Trigger( TypeSkillBase, GetObjectID(), xgc_nullptr, SkillCasting );
			}
		}
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	�����ȴ״̬ 
	//---------------------------------------------------//
	void XSkillBase::ClearCooldown( bool bTrigger )
	{
		ThisTimer().remove_event( m_hCooldownTimerHandler );
		m_hCooldownTimerHandler = INVALID_TIMER_HANDLE;

		if( bTrigger )
		{
			XActorBase* pActor = ObjectPool.GetObject< XActorBase >( GetParent() );
			if( pActor )
			{
				pActor->Trigger( TypeSkillBase, GetObjectID(), xgc_nullptr, SkillCooldown );
			}
		}
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	����ͷ�״̬
	//---------------------------------------------------//
	void XSkillBase::ClearLifetime( bool bTrigger )
	{
		ThisTimer().remove_event( m_hLifetimeTimerHandler );
		m_hLifetimeTimerHandler = INVALID_TIMER_HANDLE;

		if( bTrigger )
		{
			XActorBase* pActor = ObjectPool.GetObject< XActorBase >( GetParent() );
			if( pActor )
			{
				pActor->Trigger( TypeSkillBase, GetObjectID(), xgc_nullptr, SkillOver );
			}
		}
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	���ü�������״̬ 
	//---------------------------------------------------//
	void XSkillBase::MarkCooldown( float fDelay )
	{
		ASSERT_MSG( m_hCooldownTimerHandler == INVALID_TIMER_HANDLE, _T("����������ȴ��") );
		m_hCooldownTimerHandler = ThisTimer().insert_event( bind( &XSkillBase::ResetCooldown, this, _1 ), -1, -1, TIMER_SECONDS(fDelay) );
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	���ü����ͷ�״̬ 
	//---------------------------------------------------//
	bool XSkillBase::ResetCasting( timer_t handle )
	{
		ClearCasting( true );
		return true;
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	���ü�����ȴ״̬ 
	//---------------------------------------------------//
	bool XSkillBase::ResetCooldown( timer_t handle )
	{
		ClearCooldown( true );
		return true;
	}

	//---------------------------------------------------//
	// [12/6/2010 Albert]
	// Description:	���ü����ͷ�״̬ 
	//---------------------------------------------------//
	bool XSkillBase::ResetLifetime( timer_t handle )
	{
		ClearLifetime( true );
		return true;
	}

}

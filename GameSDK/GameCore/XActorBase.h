#pragma once

#include "GameObject.h"

namespace XGC
{
	class CObjectControlerBase;
	class CORE_API XActorBase : public CGameObject
	{
		DECLARE_DYNAMICTYPE( CGameObject, TypeActorBase );
		DECLARE_ATTRIBUTE(ActorBase_attrCount);
		friend class CObjectControlerBase;
		friend class CServerMap;
	public:
		XActorBase(void);
		~XActorBase(void);

		enum ActorStatus { ActorStatus_Born, ActorStatus_Live, ActorStatus_Pain, ActorStatus_Faint, ActorStatus_Fall, ActorStatus_Abnormal, ActorStatus_Dead, };
		enum ActorEvent
		{ 
			ActorEvent_Damage	= 1,
			ActorEvent_Pain		= 2, 
			ActorEvent_Faint	= 4, 
			ActorEvent_Fall		= 8, 
			ActorEvent_Beat		= 16, 
			ActorEvent_Dead		= 32, 
		};
		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	 �ܵ�����
		//---------------------------------------------------//
		void UnderAttack( xObject hAttacker, int nDamage );

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	��ȡ�����ߵĽ�ɫ 
		//---------------------------------------------------//
		xObject GetAttacker()const{ return m_hAttacker; }

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	��ȡ�����ߵĽ�ɫ 
		//---------------------------------------------------//
		xObject GetAvengement()const{ return m_hAvengement; }

		//---------------------------------------------------//
		// [12/27/2010 Albert]
		// Description:	��ȡ���ڹ�����Ŀ�� 
		//---------------------------------------------------//
		xObject GetTarget()const{ return m_hTarget; }

		//---------------------------------------------------//
		// [12/27/2010 Albert]
		// Description:	���õ�ǰ������Ŀ�� 
		//---------------------------------------------------//
		const XVector3& SetTarget( xObject hTarget );

		//---------------------------------------------------//
		// [8/21/2009 Albert]
		// Description:	��ɫ����
		//---------------------------------------------------//
		void Dead( xObject hAttacker );

		//---------------------------------------------------//
		// [12/28/2010 Albert]
		// Description:	��ɫ���� 
		//---------------------------------------------------//
		void Relive(){ m_ActorStatus = ActorStatus_Born; }

		//---------------------------------------------------//
		// [8/24/2009 Albert]
		// Description:	��ȡ��ɫ״̬
		//---------------------------------------------------//
		ActorStatus GetActorStatus()const{ return m_ActorStatus; }

		//---------------------------------------------------//
		// [8/24/2009 Albert]
		// Description:	���ý�ɫ״̬
		//---------------------------------------------------//
		void SetActorStatus( ActorStatus status, float fTime = 0.0f, int nMode = 0 );

		//---------------------------------------------------//
		// [8/21/2009 Albert]
		// Description:	��ȡ��Ұ��
		//---------------------------------------------------//
		unsigned int GetEyeshotGroup()const{ return m_hEyeshotGroup; }

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	��ȡ���� 
		//---------------------------------------------------//
		xObject GetSkillObject( unsigned int nIndex )const
		{ 
			ASSERT_RETURN( nIndex < m_SkillContainer.size(), INVALID_OBJECT_ID );
			return m_SkillContainer[nIndex]; 
		}

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	��ȡ���ܸ��� 
		//---------------------------------------------------//
		size_t GetSkillCount()const
		{
			return m_SkillContainer.size();
		}

		//---------------------------------------------------//
		// [12/27/2010 Albert]
		// Description:	��ȡ���еĹ���λ�� 
		//---------------------------------------------------//
		const XVector3& GetEnemySit( xObject hEnemy );

		//---------------------------------------------------//
		// [12/27/2010 Albert]
		// Description:	�������λ�� 
		//---------------------------------------------------//
		void ClearEnemySit( xObject hEnemy );

		//---------------------------------------------------//
		// [12/30/2010 Albert]
		// Description:	������ 
		//---------------------------------------------------//
		void ClearAvengement( xObject hTarget = INVALID_OBJECT_ID );
	protected:
		typedef xgc_map< xObject, int >	DamageMap;
		typedef xgc_vector< xObject >	SkillContainer;
		typedef SkillContainer::iterator		SkillIterator;
		typedef SkillContainer::const_iterator	constSkillIterator;

	private:
		//---------------------------------------------------//
		// [12/23/2010 Albert]
		// Description:	���ý�ɫ״̬ 
		//---------------------------------------------------//
		bool ResetActorStatus( timer_t handle, ActorStatus eStatus );

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	������Ķ���Ϊ��ע�����ͣ�����й���
		//---------------------------------------------------//
		virtual void OnAddChild( CXObject* pChild );

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	���Ƴ��Ķ���Ϊ��ע��������б���ȡ�� 
		//---------------------------------------------------//
		virtual void OnRemoveChild( CXObject* pChild, bool bRelease );

	protected:
		_uint32			m_hEyeshotGroup;	// ��Ұ��
		timer_t			m_hResetStatusTimerHandler;
	private:
		SkillContainer	m_SkillContainer;
		DamageMap		m_DamageMap;
		ActorStatus		m_ActorStatus;		// ��ɫ״̬
		ActorStatus		m_ActorRestonStatus;// ��ɫ����״̬

		xObject			m_hAttacker;		// ������
		xObject			m_hAvengement;		// ���
		xObject			m_hTarget;			// ��ǰĿ��
		int				m_nDamageMax;
		unsigned int	m_nEnemySit[8];		// �����ߵ�λ�ã�8��
	};
}

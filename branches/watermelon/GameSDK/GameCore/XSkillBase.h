#pragma once
using namespace XGC;

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

namespace XGC
{
	class CGameObject;
	class CORE_API XSkillBase : public CXObject
	{
	public:
		DECLARE_DYNAMICTYPE( CXObject, TypeSkillBase );
		DECLARE_ATTRIBUTE( SkillBase_attrCount );

		enum { SkillBegin, SkillCasting, SkillCooldown, SkillOver };
		XSkillBase();
		~XSkillBase(void);

		//---------------------------------------------------//
		// [12/29/2010 Albert]
		// Description:	��ʼʹ�ü��� 
		//---------------------------------------------------//
		void CastSkill( xObject hTarget );

		//---------------------------------------------------//
		// [12/3/2010 Albert]
		// Description:	����Ŀ��
		// return : �����ͷ���Ҫʹ�õ�ʱ��
		//---------------------------------------------------//
		int Attack( xObject hTarget );

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	���ü����ͷ�״̬ 
		//---------------------------------------------------//
		void MarkCasting( float fDelay );

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	���ü�����ȴ״̬ 
		//---------------------------------------------------//
		void MarkCooldown( float fDelay );

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	�����Ƿ������� 
		//---------------------------------------------------//
		bool IsCasting()const{ return m_hCastingTimerHandler != INVALID_TIMER_HANDLE; }

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	�����Ƿ���ȴ�� 
		//---------------------------------------------------//
		bool IsCooldown()const{ return m_hCooldownTimerHandler != INVALID_TIMER_HANDLE; }

		//---------------------------------------------------//
		// [12/27/2010 Albert]
		// Description:	�����Ƿ����
		//---------------------------------------------------//
		bool IsOver()const{ return m_hLifetimeTimerHandler == INVALID_TIMER_HANDLE; }

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	�������״̬ 
		//---------------------------------------------------//
		void ClearCasting( bool bTrigger = false );

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	�����ȴ״̬ 
		//---------------------------------------------------//
		void ClearCooldown( bool bTrigger = false );

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	����ͷ�״̬
		//---------------------------------------------------//
		void ClearLifetime( bool bTrigger = false );

	protected:
		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	���ü����ͷ�״̬ 
		//---------------------------------------------------//
		bool ResetCasting( timer_t handle );

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	���ü�����ȴ״̬ 
		//---------------------------------------------------//
		bool ResetCooldown( timer_t handle );

		//---------------------------------------------------//
		// [12/6/2010 Albert]
		// Description:	���ü����ͷ�״̬ 
		//---------------------------------------------------//
		bool ResetLifetime( timer_t handle );

		//---------------------------------------------------//
		// [12/3/2010 Albert]
		// Description:	����������ʱ����ã����������ж��Ƿ���Action����
		//---------------------------------------------------//
		virtual bool PreAddChild( CXObject* pChild );

	private:
		timer_t m_hCastingTimerHandler;
		timer_t m_hCooldownTimerHandler;
		timer_t m_hLifetimeTimerHandler;
	};
}

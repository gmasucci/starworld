#pragma once
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

class CGameActor;
class CORE_API CGameActorAI
{
public:
	CGameActorAI( CGameActor *pActor );
	virtual ~CGameActorAI(void);

	enum EN_EVENT_CODE
	{
		event_collision,
		event_active_actor,	// NPC����Ҽ���
		event_answer_actor,	// �ش�npc������
		event_actor_born,			// ��ɫ����
		event_actor_enter_eyeshot,	// ��ɫ������Ұ
		event_actor_leave_eyeshot,	// ��ɫ�뿪��Ұ
		event_actor_hit,			// ��ɫ�ܻ��¼�
		event_actor_dead,			// ��ɫ����
		event_actor_step,		// ��ɫ�ƶ�
		event_actor_arrived = 64,	// ��ɫ����Ŀ���
	};

	enum EN_ACTOR_HIT_SUBCODE
	{
		actor_hit_none = 0,
		actor_hit_normal = XGC_BIT(1),
		actor_hit_beatback = XGC_BIT(2),		// ��ɫ�����¼�
	};

	//////////////////////////////////////////////////////////////////////////
	//	˼��
	//	pObj	:	˼���Ķ��󣬿������Լ����ֵ
	//  nEvent	:	˼�����¼�
	//	lpBuf	:	�����¼���Ŵ����һЩ����
	//	nSize	:	lpBuf�ĳ���
	//////////////////////////////////////////////////////////////////////////
	virtual bool Think( _uint32 nObjID, int nEvent, long_ptr lParam ) = 0;

	//---------------------------------------------------//
	// [8/21/2009 Albert]
	// Description:	������Ұ��ID
	//---------------------------------------------------//
	void SetEyeshotGroup( _uint32 nEyeshotGroup );

protected:
	CGameActor*	m_pMaster;
};

#pragma once

#include "GameObject.h"
class CGameActorAI;
class CORE_API CGameActor : public XObjOnlyTypeT< CGameActor, CDynamicObject, TypeGameActor >
{
friend class CGameActorAI;
public:
	CGameActor(void);
	~CGameActor(void);

	enum ActorStatus { status_live, status_dead, };

	bool Think( int nObjID, int nEvent, long_ptr lParam );

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	���������Ұ
	// pObject	:	������Ұ�Ķ���ָ��
	//---------------------------------------------------//
	virtual void EnterEyeshot( CDynamicObject* pObject );

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	�����뿪��Ұ
	// pObject	:	�뿪��Ұ�Ķ���ָ��
	//---------------------------------------------------//
	virtual void LeaveEyeshot( CDynamicObject* pObject );

	//---------------------------------------------------//
	// [9/9/2009 Albert]
	// Description:	����Ŀ��ʱ����
	//---------------------------------------------------//
	virtual void OnArrived( const XVector3& vPosition );

	//---------------------------------------------------//
	// [9/10/2009 Albert]
	// Description:	ÿ���ƶ������
	//---------------------------------------------------//
	virtual void OnStep( const XVector3& vPosition );

	//---------------------------------------------------//
	// [8/21/2009 Albert]
	// Description:	��ɫ����
	//---------------------------------------------------//
	void Dead( _uint32 hAttacker );

	//---------------------------------------------------//
	// [8/21/2009 Albert]
	// Description:	��ȡ��ɫ��AI�ӿ�
	//---------------------------------------------------//
	CGameActorAI*	GetActorAI()const{ return m_pAI; }

	//---------------------------------------------------//
	// [8/24/2009 Albert]
	// Description:	��ȡ��ɫ״̬
	//---------------------------------------------------//
	ActorStatus GetActorStatus()const{ return m_ActorStatus; }

	//---------------------------------------------------//
	// [8/24/2009 Albert]
	// Description:	���ý�ɫ״̬
	//---------------------------------------------------//
	void SetActorStatus( ActorStatus status ){ m_ActorStatus = status; }

	//---------------------------------------------------//
	// [8/21/2009 Albert]
	// Description:	��ȡ��Ұ��
	//---------------------------------------------------//
	_uint32	GetEyeshotGroup()const{ return m_hEyeshotGroup; }

protected:
	virtual void OnDead( _uint32 hAttacker );
	//---------------------------------------------------//
	// [8/10/2009 Albert]
	// Description:	AI���
	//---------------------------------------------------//
	void AttachAI( CGameActorAI* pAI ){ ASSERT( m_pAI == NULL ); m_pAI = pAI; }
	CGameActorAI*	DetachAI(){ CGameActorAI* pTmpAI = m_pAI; m_pAI = NULL; return pTmpAI; }

protected:
	_uint32		m_hEyeshotGroup;	// ��Ұ��
	_uint32		m_hCurrentSkill;
	ActorStatus	m_ActorStatus;		// ��ɫ״̬
private:
	CGameActorAI*	m_pAI;			// �����¼�����ӿ�
};

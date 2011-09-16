#pragma once
#include "XFsmBase.h"

namespace XGC
{
	class CGameObject;
	class CObjectMoveControler;
	struct CORE_API MoveStateParams : public StateParams
	{
		XVector3	Position;	// Ŀ������
		float		fSpeed;		// �ƶ��ٶ�
		float		fDelay;		// �ӳ�ʱ��
	};

	class CORE_API CMoveState :	public CStateBase
	{
	public:
		CMoveState( byte_t code, const GuardFunctor& Fn );
		~CMoveState(void);

	protected:
		//---------------------------------------------------//
		// [11/24/2010 Albert]
		// Description:	�������¼�����ʱ
		//---------------------------------------------------//
		void OnTrigger( xObject hSource, xObject hDest, long_ptr lParam, int_ptr wParam );

		//--------------------------------------------------------//
		//	created:	21:1:2010   14:33
		//	filename: 	State
		//	author:		Albert.xu
		//
		//	purpose:	����״̬ǰ����
		//	pEntity:	����״̬�Ķ���ָ��
		//	return:		TRUE ����״̬�ɹ��� false����״̬ʧ��
		//--------------------------------------------------------//
		virtual bool OnEnter( CGameObject& Object, StateParams& Params );

		//--------------------------------------------------------//
		//	created:	21:1:2010   17:02
		//	filename: 	State
		//	author:		Albert.xu
		//
		//	purpose:	�ж�
		//--------------------------------------------------------//
		virtual void OnInterrupt();

		//--------------------------------------------------------//
		//	created:	21:1:2010   17:02
		//	filename: 	State
		//	author:		Albert.xu
		//
		//	purpose:	�жϻָ��ֳ�
		//--------------------------------------------------------//
		virtual void OnIret();

		//--------------------------------------------------------//
		//	created:	21:1:2010   14:35
		//	filename: 	State
		//	author:		Albert.xu
		//
		//	purpose:	�뿪״̬
		//	pEntity:	����״̬�Ķ���ָ��
		//--------------------------------------------------------//
		virtual void OnLeave( CGameObject& Object );

		//--------------------------------------------------------------//
		//	created:	2:2:2010   14:37
		//	filename: 	FsmBase.h
		//	author:		Albert.xu

		//	Description : 	��ȡ������Ϣ
		//--------------------------------------------------------------//
		virtual StateParams& GetParams(){ return mStateParams; }

	private:
		CObjectMoveControler*	m_pControler;
		MoveStateParams			mStateParams;
	};
};

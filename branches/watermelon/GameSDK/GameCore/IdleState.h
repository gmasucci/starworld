#pragma once
#include "XFsmBase.h"

namespace XGC
{
	class CGameObject;
	class CObjectIdleControler;

	struct CORE_API IdleStateParams : public StateParams
	{
		float fIdleTime;
	};

	class CORE_API CIdleState :	public CStateBase
	{
	public:
		CIdleState( byte_t code, const GuardFunctor& Fn );
		~CIdleState(void);

		enum { IdleTimeout, };
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
		CObjectIdleControler*	m_pControler;
		IdleStateParams			mStateParams;
	};
}
#include "StdAfx.h"
#include "MoveState.h"
#include "XActorBase.h"
#include "ObjectControlerBase.h"

#ifndef _USE_BOOST
	#include <functional>
	using namespace std;
	using namespace std::tr1;
	using namespace std::tr1::placeholders;
#else
	#include "boost\function.hpp"
	#include "boost\bind.hpp"
	using namespace boost;
#endif

namespace XGC
{
	CMoveState::CMoveState( byte_t code, const GuardFunctor& Fn )
		: CStateBase( eStateType_Move, code, Fn )
		, m_pControler( new CObjectMoveControler( xgc_nullptr, true ) )
	{
	}

	CMoveState::~CMoveState(void)
	{
		SAFE_DELETE( m_pControler );
	}

	//---------------------------------------------------//
	// [11/24/2010 Albert]
	// Description:	�������¼�����ʱ
	//---------------------------------------------------//
	void CMoveState::OnTrigger( xObject hSource, xObject hDest, long_ptr lParam, int_ptr wParam )
	{
		//ASSERT_RETURN( hSource == hDest, void(0) );
		switch( wParam )
		{
		case CGameObject::Event_MoveArrived:
			mStateParams.bit1 = true;
			break;
		case CGameObject::Event_MoveStop:
			mStateParams.bit2 = true;
			break;
		case CGameObject::Event_MoveStep:
			mStateParams.bit3 = true;
			break;
		}
	}

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:33
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬ǰ����
	//	pEntity:	����״̬�Ķ���ָ��
	//	return:		true ����״̬�ɹ��� false����״̬ʧ��
	//--------------------------------------------------------//
	bool CMoveState::OnEnter( CGameObject& Object, StateParams& Params )
	{
		mStateParams = static_cast< MoveStateParams& >( Params );

		Object.RegisteTrigger( TypeServerObject, bind(&CMoveState::OnTrigger, this, _1, _2, _3, _4 ) );
		m_pControler->Attach( &Object );
		m_pControler->MoveTo( mStateParams.Position, mStateParams.fSpeed, mStateParams.fDelay );

		return true;
	}

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�ж�
	//--------------------------------------------------------//
	void CMoveState::OnInterrupt()
	{
		m_pControler->PauseUpdate();
	}

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϻָ��ֳ�
	//--------------------------------------------------------//
	void CMoveState::OnIret()
	{
		m_pControler->ResumeUpdate();
	}

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:35
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�뿪״̬
	//	pEntity:	����״̬�Ķ���ָ��
	//--------------------------------------------------------//
	void CMoveState::OnLeave( CGameObject& Object )
	{
		m_pControler->Detach();
		Object.DismissTrigger( TypeServerObject, bind(&CMoveState::OnTrigger, this, _1, _2, _3, _4 ) );
	}
}

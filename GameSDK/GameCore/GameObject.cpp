#include "StdAfx.h"
#include "GameObject.h"
#include "ObjectControlerBase.h"

namespace XGC
{
	CGameObject::CGameObject()
		: CXObject( false, true, true )
		, m_pControler( xgc_nullptr )
	{
	}

	CGameObject::~CGameObject()
	{
		if( m_pControler )
		{
			m_pControler->Detach();
			m_pControler = xgc_nullptr;
		}
	}

	void CGameObject::RegisteTrigger( unsigned int nGroup, const TriggerFunctor &Call )
	{
		m_TriggerSignal.insert( TriggerSignal::value_type( nGroup, Call ) );
	}

	void CGameObject::Trigger( unsigned int nGroup, xObject hSource, long_ptr lParam, int_ptr wParam )
	{
		TriggerSignal::_Pairii i = m_TriggerSignal.equal_range( nGroup );
		for( TriggerSignal::iterator j = i.first ; j != i.second; ++j )
		{
			j->second( hSource, GetObjectID(), lParam, wParam );
		};
	}

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	���������Ұ
	// pObject	:	������Ұ�Ķ���ָ��
	//---------------------------------------------------//
	void CGameObject::EnterEyeshot( CGameObject* pObject )
	{
		Trigger( TypeServerObject, pObject->GetObjectID(), xgc_nullptr, Event_EnterEyeshot );
	}

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	�����뿪��Ұ
	// pObject	:	�뿪��Ұ�Ķ���ָ��
	//---------------------------------------------------//
	void CGameObject::LeaveEyeshot( CGameObject* pObject )
	{
		Trigger( TypeServerObject, pObject->GetObjectID(), xgc_nullptr, Event_LeaveEyeshot );
	}
}

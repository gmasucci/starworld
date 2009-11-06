#include "StdAfx.h"
#include "XQuest.h"
#include "GameActor.h"

CXQuestObjective::CXQuestObjective( _string strName, _uint16 nFinishCount )
: m_strName( strName )
, m_nFinishCount( nFinishCount )
, m_nCount( 0 )
, m_nSerial( 0 )
{

}

CXQuestObjective::~CXQuestObjective(void)
{

}

bool CXQuestObjective::Increment()
{
	++m_nCount;
	if( m_nCount == m_nFinishCount )
	{
		CXQuest *pQuest = static_cast< CXQuest* >( CXObjectList::GetInstance().GetObj( GetParent() ) );
		if( pQuest )
		{
			pQuest->ObjectiveNotify( m_nSerial );
		}
		return true;
	}
	return false;
}

int CXQuestObjective::OnKill( _uint32 hSource, long_ptr lParam )
{
	CGameActor *pActor = static_cast< CGameActor* >( CXObjectList::GetInstance().GetObj( (_uint32)lParam, TypeGameActor ) );
	if( pActor )
	{
		_lpctstr name = pActor->GetStrAttrib( _T("name") );
		ASSERT_POINTER( name );
		if( name && m_strName == name && Increment() )
		{
			// ��������򽫴�����ժ��
			ThisTriggerMgr()->DismissTrigger( hSource, 0, this, &CXQuestObjective::OnKill );
		}
	}
	return false;
}

int CXQuestObjective::OnItem( _uint32 hSource, long_ptr lParam )
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////////
CXQuest::CXQuest(void)
: m_FinishMask( 0 )
, m_Mask( 0 )
{
}

CXQuest::~CXQuest(void)
{
}

void CXQuest::OnAddChild( CXObject* pChild )
{
	int nChildCount = GetChildrenCount( TypeQuestObjective );
	if( nChildCount >= 16 )
	{
		XGC_WRITELOG( _T("ServerCore"), _T("����Ŀ���Ѿ�������������.") );
		return;
	}

	ASSERT( pChild && pChild->IsTypeOf(TypeQuestObjective) );
	CXQuestObjective* pObjective = static_cast< CXQuestObjective* >( pChild );
	pObjective->m_nSerial = (1<<nChildCount);
	m_FinishMask |= pObjective->m_nSerial;
}

void CXQuest::ObjectiveNotify( _uint16 nSerial )
{
	m_Mask |= nSerial;
	if( m_Mask == m_FinishMask )
		OnQuestFinished();
}
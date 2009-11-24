#include "StdAfx.h"
#include "XAction.h"
namespace XGC
{
	const action_entry_ptr	FindEffectEntry( const action_map_ptr lpEntryMap, int nActionID )
	{
		size_t n = 0, count = lpEntryMap->lpMapEntryLast - lpEntryMap->lpMapEntry;

		const action_entry_ptr lpEntry = lpEntryMap->lpMapEntry;
		while( n < count )
		{
			if( lpEntry[n].nID == nActionID )
				return lpEntry + n;

			++n;
		}

		return NULL;
	}

	CXAction::CXAction()
		: m_entry_ptr( NULL )
		, m_nActionID( INVALID_ACTION_ID )	// Ч��ID
		, m_bEffected( false )	// �Ƿ��Ѿ�ת��������
	{
		memset( m_Param, 0, sizeof( m_Param ) );
	}

	CXAction::CXAction( int nActionID, const action_entry_ptr entry_ptr )
		: m_entry_ptr( entry_ptr )
		, m_nActionID( nActionID )			// Ч��ID
		, m_bEffected( false )	// �Ƿ��Ѿ�ת��������
	{
		ZeroMemory( m_Param, sizeof( m_Param ) );	// Ч������
	}

	CXAction::CXAction( const CXAction& rsh )
		: m_entry_ptr( rsh.m_entry_ptr )
		, m_nActionID( rsh.m_nActionID )	// Ч��ID
		, m_bEffected( false )	// �Ƿ��Ѿ�ת��������
	{
		memcpy( m_Param, rsh.m_Param, sizeof( m_Param ) );
	}

	CXAction::~CXAction(void)
	{
	}

	const action_entry_ptr CXAction::getActionEntry()const
	{
		return m_entry_ptr;
	}

	int CXAction::DoAction( CXObject* pObj, bool bRemove )
	{
		// ASSERT_MSG( (m_bEffected && bRemove) || (!m_bEffected && !bRemove), _T("�ظ��Ƴ�����.") );
		if( (m_bEffected && !bRemove) || ( !m_bEffected && bRemove) )
			return false;

		// ���Լ����ϵ����òű��Ч��ת��
		int flag = (GetParent() == pObj->GetObjID())?1:0;

		// ��ʼ��������
		if( m_entry_ptr && pObj->IsTypeOf( m_entry_ptr->nDestType ) && m_entry_ptr->pDoAction )
		{
			//( this->*lpEntry->pDoEffect )( pObj, lpEntry->nAttribID, lpEntry->nType, lpEntry->bPersent, bRemove );
			int ret = (*m_entry_ptr->pDoAction)( this, pObj, m_entry_ptr->ConstParam, m_entry_ptr->nDestType, bRemove );
			//const action_entry_ptr pEffectNode = lpEntry;//AllocEffectItem();
			//m_NodeList.push_back( pEffectNode );
			m_bEffected += (bRemove?-flag:flag);	// ��־�Լ���Ч���Ѿ���ת����
			return ret;
		}
		//ASSERT_MSG( !m_NodeList.empty(), "none effect be found" );
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// nIdx		:	����
	// fValue	:	ֵ
	// return	:	�Ƿ����óɹ�
	//////////////////////////////////////////////////////////////////////////
	bool CXAction::SetParam( int nIdx, CXVariant fValue )
	{
		ASSERT( nIdx >= 0 && nIdx < ACTION_PARAM_MAX );
		if( nIdx < 0 || nIdx >= ACTION_PARAM_MAX )
		{
			return false;
		}

		m_Param[nIdx] = fValue;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// nIdx		:	����
	// fValue	:	ֵ
	// return	:	�Ƿ����óɹ�
	//////////////////////////////////////////////////////////////////////////
	bool CXAction::SetOwnerParam( int nIdx, CXVariant fValue )
	{
		ASSERT( nIdx >= 0 && nIdx < ACTION_PARAM_MAX );
		if( nIdx < 0 || nIdx >= ACTION_PARAM_MAX )
		{
			return false;
		}

		m_ParamEx[nIdx] = fValue;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// fValue	:	ֵ����ָ��
	// nFirstIdx:	��ʼ����
	// nEndIdx	:	��������
	// return	:	�Ƿ����óɹ�
	// remark	:	nFirstIdx ���Դ��� nEndIdx ����fValue��ֵ�����������롣
	//////////////////////////////////////////////////////////////////////////
	bool CXAction::SetParam( const CXVariant *fValue, int nFirstIdx, int nEndIdx )
	{
		if( nFirstIdx > nEndIdx ) std::swap( nFirstIdx, nEndIdx );
		ASSERT( nFirstIdx >= 0 && nEndIdx < ACTION_PARAM_MAX );
		if( nFirstIdx < 0 || nEndIdx >= ACTION_PARAM_MAX )
		{
			return false;
		}

		for( int i = nFirstIdx; i != nEndIdx + 1; ++i )
		{
			m_Param[i] = *fValue++;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// fValue	:	ֵ����ָ��
	// nFirstIdx:	��ʼ����
	// nEndIdx	:	��������
	// return	:	�Ƿ����óɹ�
	// remark	:	nFirstIdx ���Դ��� nEndIdx ����fValue��ֵ�����������롣
	//////////////////////////////////////////////////////////////////////////
	bool CXAction::SetOwnerParam( const CXVariant *fValue, int nFirstIdx, int nEndIdx )
	{
		if( nFirstIdx > nEndIdx ) std::swap( nFirstIdx, nEndIdx );
		ASSERT( nFirstIdx >= 0 && nEndIdx < ACTION_PARAM_MAX );
		if( nFirstIdx < 0 || nEndIdx >= ACTION_PARAM_MAX )
		{
			return false;
		}

		for( int i = nFirstIdx; i != nEndIdx + 1; ++i )
		{
			m_ParamEx[i] = *fValue++;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Status class
	//////////////////////////////////////////////////////////////////////////
	CXStatus::CXStatus( int nActionID, const action_entry_ptr emap )
		: BaseClass( nActionID, emap )
		, m_hTimerHandler( INVALID_TIMER_HANDLE )
	{

	}

	CXStatus::~CXStatus()
	{
		if( m_hTimerHandler != INVALID_TIMER_HANDLE )
		{
			ThisTimer()->remove_event( m_hTimerHandler );
			m_hTimerHandler = INVALID_TIMER_HANDLE;
		}
	}

	//---------------------------------------------------//
	// [9/13/2009 Albert]
	// Description:	��װ��ʱ��
	//---------------------------------------------------//
	void CXStatus::InstallTimer( _uint32 nCount, float fInterval, float fDelay )
	{
		// ����״̬���ö�ʱ��
		m_hTimerHandler = ThisTimer()->insert_event( 
			TimerFunction( this, &CXStatus::OnTimer ), 
			nCount, 
			TIMER_SECONDS( fInterval ),
			TIMER_SECONDS( fDelay ) );
	}

	bool CXStatus::OnTimer( unsigned int handle, unsigned short &repeat, unsigned int &timer )
	{
		// CLogger::GetInstance(_T("Log"))->WriteLog( _T("��ɫ[%08x]״̬���� Count = %d"), GetParent(), repeat );
		CXObject *pObj = CXObjectPool::GetInstance().GetObj( GetParent() );
		if( pObj )
		{
			m_bEffected = 0;
			DoAction( pObj, repeat == 0 );
		}

		if( repeat == 0 )
		{
			Destroy();
		}
		return false;
	}
}

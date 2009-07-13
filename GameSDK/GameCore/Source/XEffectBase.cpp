#include "StdAfx.h"
#include ".\xeffectbase.h"

CXEffect::CXEffect( int nEffectID )
{
	m_nOwnerID		= INVALID_OBJID;		// ������ID
	m_nUserID		= INVALID_OBJID;		// ʹ����ID
	m_nEffectID		= nEffectID;			// Ч��ID
	m_nLv			= 0;
	ZeroMemory( m_fParam, sizeof( m_fParam ) );	// Ч������

	m_bEffected		= false;	// �Ƿ��Ѿ�ת��������
}

CXEffect::~CXEffect(void)
{
	//CNodeList::iterator iter = m_NodeList.begin();
	//while( iter !=m_NodeList.end() )
	//{
	//	delete (*iter);
	//	iter++;
	//}
	m_NodeList.clear();
}

const EFFECT_ENTRY CXEffect::_effectEntry[] =
{
	{ -1, -1, -1, 0, NULL, -1 }     // nothing here
};

const EFFECT_MAP CXEffect::_effectMAP = 
{
	NULL, 
	&_effectEntry[0],
};

const EFFECT_MAP*	CXEffect::getEffectMap() const
{
	return &CXEffect::_effectMAP;
}

const EFFECT_ENTRY*	CXEffect::FindEffectEntry( const EFFECT_ENTRY* lpEntry, int nEffectID )
{
	while( lpEntry->nID != INVALID_EFFECTID )
	{
		if( lpEntry->nID == nEffectID )	return lpEntry;
		lpEntry++;
	}
	return NULL;    // not found
}

int CXEffect::DoEffect( CXObject* pObj, bool bRemove )
{
	ASSERT_MSG( (m_bEffected && bRemove) || (!m_bEffected && !bRemove), _T("�ظ��Ƴ�����.") );
	// �ظ���������,�����Ƴ�������û������ǰ
	int nEffectID = m_nEffectID;
	// ��Ч��������ɺ󣬿��ܻ�������һ������������Ч����
	if( m_NodeList.empty() )
	{
		// ��ʼ��������
		const EFFECT_MAP*	pMessageMap;
		const EFFECT_ENTRY*	lpEntry;
		while( nEffectID != INVALID_EFFECTID )
		{
			for( pMessageMap = getEffectMap(); pMessageMap != NULL; pMessageMap = pMessageMap->pBaseMap )
			{
				lpEntry = FindEffectEntry( pMessageMap->lpMapEntry, nEffectID );
				if( lpEntry && pObj->IsType( lpEntry->nType ) )
				{
					( this->*lpEntry->pDoEffect )( pObj, lpEntry->nAttribID, lpEntry->nType, lpEntry->bPersent, bRemove );
					const EFFECT_ENTRY* pEffectNode = lpEntry;//AllocEffectItem();
					//pEffectNode->nID		= lpEntry->nID;
					//pEffectNode->nAttribID	= lpEntry->nAttribID;
					//pEffectNode->nType		= lpEntry->nType;
					//pEffectNode->bPersent	= lpEntry->bPersent;
					//pEffectNode->pDoEffect	= lpEntry->pDoEffect;
					//pEffectNode->nNextEffID = lpEntry->nNextEffID;

					m_NodeList.push_back( pEffectNode );

					m_bEffected += (bRemove?-1:1);	// ��־�Լ���Ч���Ѿ���ת����

					ASSERT_MSG( nEffectID != lpEntry->nNextEffID, _T("����ѭ�����ã�������������������������") );
					nEffectID = lpEntry->nNextEffID;	// ��һ������
					break;
				}
				else
				{
					nEffectID = INVALID_EFFECTID;
				}
			}
		}
		//ASSERT_MSG( !m_NodeList.empty(), "none effect be found" );
		return true;
	}

	// �Ѿ����������û�����NodeList�У�����ֱ�ӾͿ���ִ���ˡ������ٴ�ȥ���
	CNodeList::const_iterator citer = m_NodeList.begin();
	while( citer !=m_NodeList.end() )
	{
		nEffectID = ( this->*( ( *citer )->pDoEffect ) )( 
			pObj,
			( *citer )->nAttribID,
			( *citer )->nType,
			( *citer )->bPersent,
			bRemove );

		m_bEffected += (bRemove?-1:1);	// ��־�Լ���Ч���Ѿ���ת����
		citer++;
	}
	return true;
}

int	CXEffect::DoDefaultEffect( CXObject* pObj, int nAttribID, int nType, bool bPersent, bool bRemove )
{
	float fValue = bRemove?-m_fParam[0]:m_fParam[0];
	pObj->ChangeValue( nType, nAttribID, fValue, bPersent );

	return INVALID_EFFECTID;
}

//////////////////////////////////////////////////////////////////////////
// ���ò���
// nIdx		:	����
// fValue	:	ֵ
// return	:	�Ƿ����óɹ�
//////////////////////////////////////////////////////////////////////////
bool CXEffect::SetParam( int nIdx, float fValue )
{
	ASSERT( nIdx >= 0 && nIdx < EFFECT_PARAM_MAX );
	if( nIdx < 0 && nIdx >= EFFECT_PARAM_MAX )
	{
		return false;
	}

	m_fParam[nIdx] = fValue;

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
bool CXEffect::SetParam( float* fValue, int nFirstIdx, int nEndIdx )
{
	ASSERT( nEndIdx >= 0 && nEndIdx < EFFECT_PARAM_MAX );
	ASSERT( nFirstIdx >= 0 && nFirstIdx < EFFECT_PARAM_MAX );
	if( nFirstIdx > nEndIdx ) std::swap( nFirstIdx, nEndIdx );
	if( nEndIdx < 0 || nEndIdx >= EFFECT_PARAM_MAX )
	{
		return false;
	}

	if( nFirstIdx < 0 || nFirstIdx >= EFFECT_PARAM_MAX )
	{
		return false;
	}

	for( int i = nFirstIdx; i != nEndIdx + 1; ++i )
	{
		m_fParam[i] = *fValue++;
	}
	return true;
}

CEffectFactory& CEffectFactory::GetInstance()
{
	static CEffectFactory _inst;
	return _inst;
}

//////////////////////////////////////////////////////////////////////////
//enum EFFECT_ITEM
//{
//	EFF_ITEM_HP	=	0,
//	EFF_ITEM_HPMAX,
//	EFF_ITEM_NONE,
//	EFF_ITEM_TEST1,
//	EFF_ITEM_TEST2,
//};
//
//BEGIN_EFFECT_TABLE( CXEffectItem, CXEffect )
//	EFFECT_DEFAULT( EFF_ITEM_HP,	0, TypeItem, EFF_ITEM_HPMAX )
//	EFFECT_DEFAULT( EFF_ITEM_HPMAX, 1, TypeItem, INVALID_EFFECTID )
//	EFFECT_DEFAULT_PERSENT( EFF_ITEM_TEST1, 2, TypeItem, INVALID_EFFECTID )
//	EFFECT_DEFAULT( EFF_ITEM_TEST2, 3, TypeItem, INVALID_EFFECTID )
//	EFFECT_DEFAULT( EFF_ITEM_NONE,	0, TypeMapobj, INVALID_EFFECTID )
//END_EFFECT_TABLE()


#pragma once

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS
#pragma warning( disable:4251 )

#include "stdafx.h"
#include "XObject.h"
#include <list>
/*********************************************************************/
/*
    Create:     2006/01/041:2006   17:46
    FileName:   e:\Projects\CaseProject\CaseProject\XEffectBase.h
    Author:     Albert xu

    Levle:      0
    Company:    ������˹
*/
/*********************************************************************/
/************************************************************************/
/* ʵ��������ϵͳ������ϵͳ����������˶�����ϵͳ���õĵײ���롣ͨ��һ��궨��
/* ָ�����ñ�ź����Ա�ŵĶ�Ӧ��ϵ���Լ����õĹ��̴��������ײ��ṩ��һ���򵥵�
/* ���̴����������������̴������������û����ɶ��塣ͬʱ������ϵͳ��Ӱ����
/* ����ʹ�������þ�������ԣ�����ĳ������ɺ󴥷���һ���ü����Զ���ʩ��Ӱ�죬����
/* ����HP��ͬʱҲ����HP���ֵ������������������������

/* ���ú����ԵĶ�Ӧ��ϵ�Ƕ��һ�ģ���ͬ������Ч�����ܶ�Ӱ��ͬһ������ֵ
/************************************************************************/
#define DECLARE_EFFECT_MAP()\
private:\
	static	const EFFECT_ENTRY	_effectEntry[];\
protected:\
	static	const EFFECT_MAP	_effectMAP;\
	virtual const EFFECT_MAP*	getEffectMap() const;\

#define BEGIN_EFFECT_TABLE( theClass, baseClass )\
	const EFFECT_MAP* theClass::getEffectMap() const\
	{ return &theClass::_effectMAP; };\
	__declspec( selectany ) const EFFECT_MAP theClass::_effectMAP = \
	{ &baseClass::_effectMAP, &theClass::_effectEntry[0] };\
	__declspec( selectany ) const EFFECT_ENTRY theClass::_effectEntry[] = \
	{\

#define EFFECT_DECLEAR( EffectID, AttribID, AttrType, IsPersent, pDoEffect, nNextEffectID )\
	{ EffectID, AttribID, AttrType, IsPersent,\
	( DoEffectFunc )( static_cast< bool ( CXEffect::* )( CXObject*, int, int, bool, bool ) >( &pDoEffect ) ), \
	nNextEffectID },\

#define EFFECT_DEFAULT( EffectID, AttribID, AttrType, nNextEffectID )\
	{ EffectID, AttribID, AttrType, false,\
	( DoEffectFunc )( static_cast< bool ( CXEffect::* )( CXObject*, int, int, bool, bool ) >( &DoDefaultEffect ) ), \
	nNextEffectID },\

#define EFFECT_DEFAULT_PERSENT( EffectID, AttribID, AttrType, nNextEffectID )\
	{ EffectID, AttribID, AttrType, true,\
	( DoEffectFunc )( static_cast< bool ( CXEffect::* )( CXObject*, int, int, bool, bool ) >( &DoDefaultEffect ) ), \
	nNextEffectID },\

#define END_EFFECT_TABLE()\
	{ -1, -1, -1, 0, NULL, -1 } };\

// ����ָ������ָ��
class CORE_API CXEffect;
typedef int ( CXEffect::* DoEffectFunc )( CXObject*, int, int, bool, bool );

struct EFFECT_ENTRY
{
	int				nID;		// effect id		Ч�����
	int				nAttribID;	// effect attrib id	���ñ��
	int				nType;		// effect type		��������
	bool			bPersent;	// effect persent	�Ƿ񰴰ٷֱȼ���
	DoEffectFunc	pDoEffect;	// effect func		Ч��ִ�к���
	int				nNextEffID;	// ����Ч��ID
};

// ��Ϣӳ���ṹ
struct EFFECT_MAP
{
	const EFFECT_MAP*	pBaseMap;
	const EFFECT_ENTRY*	lpMapEntry;
};

enum	EFFECT_OWNER_TYPE
{
	EN_OWNER_OBJECT,
	EN_OWNER_ITEM,
	EN_OWNER_STATE,
};

#define EFFECT_PARAM_MAX 6
#define INVALID_EFFECTID -1
class CORE_API CXEffect	:	public XObjTypeT< CXObject, TypeEffect >
{
DECLARE_EFFECT_MAP()
private:
	int		m_nOwnerID;		// ������ID
	int		m_nUserID;		// ʹ����ID
	int		m_nEffectID;	// Ч��ID
	float	m_fParam[EFFECT_PARAM_MAX];	// Ч������

	BYTE	m_nLv;			// Ч���ȼ�
	BYTE	m_nOwnerType;	// ���������� -���� -״̬��

protected:
	typedef std::list< const EFFECT_ENTRY* >	CNodeList;	// ���ýڵ��б�

	CNodeList	m_NodeList;
	int			m_bEffected;	// �Ƿ��Ѿ�ת��������

	const EFFECT_ENTRY*	FindEffectEntry( const EFFECT_ENTRY* lpEntry, int nEffectID );
	EFFECT_ENTRY* AllocEffectItem(){ return new EFFECT_ENTRY; }

public:
	explicit CXEffect( int nEffectID );
	~CXEffect(void);

	void	SetOwner( int nID ) { m_nOwnerID = nID; }
	int		GetOwner()const { return m_nOwnerID; }

	void	SetUser( int nID ) { m_nUserID = nID; }
	int		GetUser()const { return m_nUserID; }

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// nIdx		:	����
	// fValue	:	ֵ
	// return	:	�Ƿ����óɹ�
	//////////////////////////////////////////////////////////////////////////
	bool	SetParam( int nIdx, float fValue );

	//////////////////////////////////////////////////////////////////////////
	// ���ò���
	// fValue	:	ֵ����ָ��
	// nFirstIdx:	��ʼ����
	// nEndIdx	:	��������
	// return	:	�Ƿ����óɹ�
	// remark	:	nFirstIdx ���Դ��� nEndIdx ����fValue��ֵ�����������롣
	//////////////////////////////////////////////////////////////////////////
	bool	SetParam( float* fValue, int nFirstIdx, int nEndIdx );

	// ִ������
	int		DoEffect( CXObject* pObj, bool bRemove = false );

	// Ĭ�ϵ����Լ��㺯��
	int		DoDefaultEffect( CXObject* pObj, int nAttribID, int nType, bool bPersent, bool bRemove );
};

#define REGIST_EFFECT( TYPE, LABLE )	CEffectRegister< TYPE > g_##__FILE__##LABLE( #LABLE )
#define CREATE_EFFECT( LABLE, EFFID )	CEffectFactory::GetInstance().Creator( (#LABLE), (EFFID) )
#define CREATE_EFFECT_PARAM( LABLE, EFFID, PARAM )	CEffectFactory::GetInstance().Creator( (#LABLE), (EFFID), (PARAM) )
#define CREATE_EFFECT_PARAMARRAY( LABLE, EFFID, PARAM, COUNT )	CEffectFactory::GetInstance().Creator( (#LABLE), (EFFID), (PARAM), (COUNT) )
class CORE_API CEffectFactory
{
protected:
	CEffectFactory(void){};
	~CEffectFactory(void){};

public:
	// ��������
	typedef CXEffect* (*pfnCreator)( int nEffectID, float* fParam, int nCount );

	// ����ģʽ
	static CEffectFactory& GetInstance();

	// ע��һ������
	void Regist( LPCTSTR pszLable, pfnCreator pfunc )
	{
		CEffectFactoryMap::const_iterator citer = m_FactoryMap.find( pszLable );
		if( citer != m_FactoryMap.end() )	return;

		m_FactoryMap[pszLable] = pfunc;
	}

	CXEffect* Creator( LPCTSTR pszLable, int nEffectID, float* fParam = NULL, int nCount = 0 )
	{
		CEffectFactoryMap::const_iterator citer = m_FactoryMap.find( pszLable );
		if( citer != m_FactoryMap.end() )
		{
			return citer->second( nEffectID, fParam, nCount );
		}
		return NULL;
	}

	CXEffect* Creator( LPCTSTR pszLable, int nEffectID, float fParam )
	{
		CEffectFactoryMap::const_iterator citer = m_FactoryMap.find( pszLable );
		if( citer != m_FactoryMap.end() )
		{
			return citer->second( nEffectID, &fParam, 1 );
		}
		return NULL;
	}

private:
	typedef std::map< _string, pfnCreator >	CEffectFactoryMap;
	CEffectFactoryMap	m_FactoryMap;
};

template< typename TEffectType >
class CEffectRegister
{
public:
	explicit CEffectRegister( LPCTSTR pszLable )
	{
		CEffectFactory::GetInstance().Regist( pszLable, CreateEffect );
	}

	static CXEffect* CreateEffect( int nEffectID, float* fParam, int nCount )
	{
		TEffectType* pEffect = new TEffectType( nEffectID );
		ASSERT( !fParam || ( fParam && nCount > 0 && nCount <= EFFECT_PARAM_MAX ) );
		if( fParam && nCount > 0 && nCount <= EFFECT_PARAM_MAX )
		{
			pEffect->SetParam( fParam, 0, nCount - 1 );
		}
		return pEffect;
	}
};

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* һ������ϵͳ����ʾ
/************************************************************************/
//class CXEffectItem	:	public CXEffect
//{
//DECLARE_EFFECT_MAP()
//public:
//	CXEffectItem( int nEffectID ):CXEffect( nEffectID ){};
//	~CXEffectItem(){};
//};

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* ״̬ϵͳ��һ�����ü�����ķ�װ
/************************************************************************/
template< typename T >
class CXStatus
{
protected:
	int		m_nOwnerID;
	int		m_nUserID;
	int		m_nStatusID;
	int		m_nLv;

	typedef std::list< CXEffect* >	CNodeList;	// ���ýڵ��б�

	CNodeList	m_NodeList;
	bool		m_bStated;	// �Ƿ��Ѿ�ת��������

public:
	CXStatus( int nStatueID );
	virtual ~CXStatus();

	CXEffect*	AddEffect( int nEffectID );

	int		DoStatus( CXObject* pObj, bool bRemove = false );
	int		DoDefaultStatus( CXObject* pObj, bool bRemove );
};

//////////////////////////////////////////////////////////////////////////
// CXStatus
template< typename T>
CXStatus< typename T >::CXStatus( int nStatueID )
{
	//T* p;static_cast< CXEffect* >( p );
	m_nOwnerID	= INVALID_OBJID;
	m_nUserID	= INVALID_OBJID;
	m_nStatusID = nStatueID;
	m_nLv		= 0;
}

template< typename T>
CXStatus< typename T >::~CXStatus()
{
	//CNodeList::const_iterator citer = m_NodeList.begin();
	while( !m_NodeList.empty() )
	{
		CNodeList::iterator iter = m_NodeList.begin();
		delete (*iter);
		m_NodeList.pop_front();
	}
	m_NodeList.clear();
}

template< typename T>
CXEffect* CXStatus< typename T >::AddEffect( int nEffectID )
{
	CXEffect* pEffect = new T( nEffectID );
	if( !pEffect )	return NULL;

	pEffect->m_nOwnerID = m_nOwnerID;
	pEffect->m_nUserID	= m_nUserID;
	pEffect->m_nLv		= m_nLv;
	pEffect->m_nOwnerType = EN_OWNER_STATE;
	m_NodeList.push_back( pEffect );
	return pEffect;
}

template< typename T>
int CXStatus< typename T >::DoStatus( CXObject* pObj, bool bRemove /* = false  */ )
{
	CNodeList::const_iterator citer = m_NodeList.begin();
	while( citer != m_NodeList.end() )
	{
		(*citer)->DoEffect( pObj, bRemove );
		citer++;
	}
	return true;
}

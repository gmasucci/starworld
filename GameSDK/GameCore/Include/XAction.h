#pragma once

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS
#pragma warning( disable:4251 )

#include "XObject.h"
#include <list>
/*********************************************************************/
/*
    Create:     2006/01/041:2006   17:46
    FileName:   XEffectBase.h
    Author:     Albert xu
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
namespace XGC
{
	// ����ָ������ָ��
	class CORE_API CXAction;
	typedef int (*DoActionFunc_c)( const CXAction*, CXObject*, CXVariant[3], int, bool );

	typedef struct ACTION_ENTRY
	{
		unsigned int	nID;			// id			Ч�����
		unsigned int	nDestType;		// type			Ŀ������
		CXVariant		ConstParam[3];	// param		��̬����
		DoActionFunc_c	pDoAction;		// func			����ִ�к���
	} action_entry, *action_entry_ptr;

	// ��Ϣӳ���ṹ
	typedef struct ACTION_MAP
	{
		const ACTION_MAP	*pBaseMap;
		action_entry_ptr	lpMapEntry;
		action_entry_ptr	lpMapEntryLast;
	} action_map, *action_map_ptr;

	CORE_API const action_entry_ptr	FindEffectEntry( const action_map_ptr lpEntryMap, int nActionID );

	#define ACTION_PARAM_MAX 3
	#define INVALID_ACTION_ID -1

	class CORE_API CXAction	:	public XObjOnlyTypeT< CXAction, CXObject, TypeGameAction >
	{
	//DECLARE_EFFECT_MAP()
	private:
		_uint32	m_nActionID;	// Ч��ID

		//_byte	m_nLv;			// Ч���ȼ�
		//_byte	m_nOwnerType;	// ���������� -���� -״̬��

		const action_entry_ptr m_entry_ptr;	// ģ�����
	protected:
		int			m_bEffected;					// �Ƿ��Ѿ�ת��������
		CXVariant	m_Param[ACTION_PARAM_MAX];		// Ч������
		CXVariant	m_ParamEx[ACTION_PARAM_MAX];	// Ч������

		CXAction();
	public:
		CXAction( int nActionID, const action_entry_ptr emap );
		CXAction( const CXAction& rsh );
		~CXAction(void);

		//////////////////////////////////////////////////////////////////////////
		// ���ò���
		// nIdx		:	����
		// fValue	:	ֵ
		// return	:	�Ƿ����óɹ�
		//////////////////////////////////////////////////////////////////////////
		bool	SetParam( int nIdx, CXVariant fValue );

		//////////////////////////////////////////////////////////////////////////
		// ���ò���
		// nIdx		:	����
		// fValue	:	ֵ
		// return	:	�Ƿ����óɹ�
		//////////////////////////////////////////////////////////////////////////
		bool	SetOwnerParam( int nIdx, CXVariant fValue );

		//////////////////////////////////////////////////////////////////////////
		// ��ȡ����
		// nIdx		:	����
		// fValue	:	ֵ
		// return	:	�Ƿ����óɹ�
		//////////////////////////////////////////////////////////////////////////
		CXVariant	GetParam( int nIdx )const{ ASSERT( nIdx >=0 && nIdx < ACTION_PARAM_MAX ); return m_Param[nIdx]; }

		//////////////////////////////////////////////////////////////////////////
		// ��ȡ����
		// nIdx		:	����
		// fValue	:	ֵ
		// return	:	�Ƿ����óɹ�
		//////////////////////////////////////////////////////////////////////////
		CXVariant	GetOwnerParam( int nIdx )const{ ASSERT( nIdx >=0 && nIdx < ACTION_PARAM_MAX ); return m_ParamEx[nIdx]; }

		//////////////////////////////////////////////////////////////////////////
		// ���ò���
		// fValue	:	ֵ����ָ��
		// nFirstIdx:	��ʼ����
		// nEndIdx	:	��������
		// return	:	�Ƿ����óɹ�
		// remark	:	nFirstIdx ���Դ��� nEndIdx ����fValue��ֵ�����������롣
		//////////////////////////////////////////////////////////////////////////
		bool	SetParam( const CXVariant *fValue, int nFirstIdx, int nEndIdx );

		//////////////////////////////////////////////////////////////////////////
		// ���ò���
		// fValue	:	ֵ����ָ��
		// nFirstIdx:	��ʼ����
		// nEndIdx	:	��������
		// return	:	�Ƿ����óɹ�
		// remark	:	nFirstIdx ���Դ��� nEndIdx ����fValue��ֵ�����������롣
		//////////////////////////////////////////////////////////////////////////
		bool	SetOwnerParam( const CXVariant *fValue, int nFirstIdx, int nEndIdx );

		//---------------------------------------------------//
		// [9/19/2009 Albert]
		// Description:	����״̬ת�Ʊ�־
		//---------------------------------------------------//
		void	ResetFlag(){ m_bEffected = 0; }

		// ִ������
		int		DoAction( CXObject* pObj, bool bRemove = false );

	protected:
		const action_entry_ptr getActionEntry()const;
	};

	class CORE_API CXStatus :	public  XObjOnlyTypeT< CXStatus, CXAction, TypeStatus >
	{
	private:
		_uint32	m_hTimerHandler;
	public:
		explicit CXStatus( int nActionID, const action_entry_ptr emap );
		explicit CXStatus( const CXStatus& rsh );
		~CXStatus();

		//---------------------------------------------------//
		// [9/13/2009 Albert]
		// Description:	��װ��ʱ��
		//---------------------------------------------------//
		void InstallTimer( _uint32 nCount, float fInterval, float fDelay = 0 );

	protected:
		//---------------------------------------------------//
		// [9/13/2009 Albert]
		// Description:	��ʱ���¼�
		//---------------------------------------------------//
		bool OnTimer( unsigned int handle, unsigned short &repeat, unsigned int &timer );
	};
}
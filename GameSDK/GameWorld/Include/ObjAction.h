#pragma once
#ifdef GAMEWORLD_EXPORTS
#define GAMEWORLD_API __declspec(dllexport)
#else
#define GAMEWORLD_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#include "XObjectAI.h"
#include "GameObject.h"

//// ��������
//class CMap;
//enum EN_ACTION
//{
//	action_None	= 0,
//	action_Say,				// ����[Message]
//	action_ChangeDir,		// ���ķ���[Dir]
//	action_WaitUserInput,	// �ȴ��û�����
//	action_Moveto,			// [����������[x,y]]
//	action_Lookfor,			// Ѱ����Χ����
//};
//
//class GAMEWORLD_API CActionChangeDir :	public CAction
//{
//public:
//	CActionChangeDir( float fDirection, int nHostID );
//	~CActionChangeDir(void);
//
//	virtual BOOL Execute( const CMap& Map );		// ִ�ж���
//private:
//	float	m_fDir;
//};
//
//class GAMEWORLD_API CActionWaitUserInput	:	public CAction
//{
//public:
//	CActionWaitUserInput();
//	virtual	BOOL Execute( const CMap& Map );
//
//	static void SetUserInput( int nInput ){ m_nUserInput = nInput; }
//private:
//	static int	m_nUserInput;
//};
//
//class GAMEWORLD_API CActionLookfor	:	public	CAction
//{
//public:
//	CActionLookfor( int nSrcObjID );
//	virtual BOOL Execute( const CMap& Map );
//};

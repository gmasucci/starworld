#pragma once
#include "GameObject.h"
#include "XObjectAI.h"
//////////////////////////////////////////////////////////////////////////
// class CEvent
//////////////////////////////////////////////////////////////////////////
/*
// Event ����רע�����¼���ʵ�֣������ͼ�еĴ�����ΪCMapEvent������¼�����
// ���¼���������Դ���͵�һ��ָ���ص㡣�ֱ���Npc���ϵĶԻ��¼�ΪCObjEvent�����
// �¼����󣬸��¼����Դ���Դ����һ��Say:Actionͬʱ����һ��Replay:Event�¼�

// Event��ְ����ת�����ݸ���ӳ�Ĵ��������䱾���������ݽ��в�����
*/
//////////////////////////////////////////////////////////////////////////
// �¼�����
enum EN_EVENT
{
	event_None		= 0,
	event_UserInput,
	event_MapEvent,		// ��ͼ�¼�
	event_Talk,			// �Ի��¼�
	event_beCollision,	// ����ײ�¼�[]
	event_Collision,	// ��ײ�¼�[]
};

//enum EN_EVENT_TYPE
//{
//	type_event_map,
//};
//
//class CMap;
//class CGameEvent
//{
//public:
//	CEvent( int nType ): m_nType( nType ){}
//protected:
//	int m_nType;
//};
//
//class CEventPool
//{
//public:
//	CEventPool();
//	~CEventPool();
//
//	static BOOL			Initialize( int nTypeCount );
//	static CEventPool&	GetInstance( int nType );
//
//private:
//	typedef std::queue< CGameEvent* > CEventQueue;
//	CEventQueue	m_evnet_queue;
//};
//
//class CMapEvent	:	public	CGameEvent
//{
//public:
//	CMapEvnet(): CGameEvent( type_event_map ){}
//
//private:
//	virtual OnEvent();
//
//private:
//	CMap*	m_pMap;
//};
#pragma once
#ifdef GAMEWORLD_EXPORTS
#define GAMEWORLD_API __declspec(dllexport)
#else
#define GAMEWORLD_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#include "XObject.h"
class CGameObject;
//////////////////////////////////////////////////////////////////////////
/*
	�����AI��CXObjectAI��������AI�ӿڶ���һ��Think�ӿں������ú�������һ����
�������Ա�ʾ������߼���֧��ͨ��������������ݻ��壬�Բ�ͬ�Ļ���������Ӧ��AI��
��һ��Action�������С��ö�����ʵ�ʵ���Ϸ������������ṩ��Ӧ�Ĺ��ܡ���������ִ
���ߺ���Ӱ��Ľ��ܷ��������󣬶���Ŀ����������Ϊ�˶������Ը���Ŀ��Ĳ�ͬ��ʹ
�ò�ͬ���߼���֧���Լ���Ŀ�����״̬���ơ�
	��Ҫע����ǣ�Action����Ӱ��Դ��Ŀ�����Ϊ�������Ҫͬʱ�ı������������Ϊ��
��ʹ��Event����
*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��ɫ�Ķ����ӿ�
class CXObjectAI;
class CMap;
class GAMEWORLD_API CAction
{
protected:
friend class CXObjectAI;
	explicit CAction( int nSrcID, int nDescID )
		: m_nSrcID( nSrcID )
		, m_nDescID( nDescID )
	{}
	virtual ~CAction(){}

protected:
	int		m_nSrcID;		// ִ�ж����Ķ���ID
	int		m_nDescID;		// ��ԵĶ���ID������Ϊ��Чֵ

	BOOL	m_bIsFinish;

public:
	// ��Map�����úͲ���׃Map����K��ì�ܡ�Action�����؟߀�ǌ�Դ�����M�в�����
	// �K��Action�������S��׃��ԭ���������������
	virtual BOOL Execute( CMap& Map )	= 0;		// ִ�ж���
};

//////////////////////////////////////////////////////////////////////////
// �����AI�ӿ�
class GAMEWORLD_API CXObjectAI
{
public:
	CXObjectAI( int nHostID );
	CXObjectAI();

	virtual ~CXObjectAI(void);

	//////////////////////////////////////////////////////////////////////////
	/*
	//	Think �����ڲ�ΪAction�Ĳ����ء����ô�������κεط�����������Action������
	*/
	//////////////////////////////////////////////////////////////////////////
	//	˼��
	//	pObj	:	˼���Ķ��󣬿������Լ����ֵ
	//  nEvent	:	˼�����¼�
	//	lpBuf	:	�����¼���Ŵ����һЩ����
	//	nSize	:	lpBuf�ĳ���
	//////////////////////////////////////////////////////////////////////////
	virtual BOOL Think( CGameObject* pObj, int nEvent, LPCSTR lpBuf, size_t nSize ) = 0;

	// ����
	virtual void Update( float fDeltaTime ) = 0;

	void AttachHost( int nHostID ){ m_nHostID = nHostID; }
protected:

	int		m_nHostID;
};

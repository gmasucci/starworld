#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Event.h"
#include "DataHelper.h"

#ifdef GAMEWORLD_EXPORTS
#define GAMEWORLD_API __declspec(dllexport)
#else
#define GAMEWORLD_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#define		PICTURE_COUNT	5
//////////////////////////////////////////////////////////////////////////
// MapBlock ���ӹ���
class GAMEWORLD_API CMapBlock
{
public:
	BOOL	m_bAllow;		// �Ƿ�����ͨ��
	int		m_nHeight;		// �߶�
	int		m_nIntension;	// ǿ��
	POINT	m_ptPos;		// ��������
	CAnimation*	m_pAni;		// ��������������
	typedef std::list< DWORD >	CObjList;
	CObjList		m_ObjList;

	CMapBlock()
	{
		m_pAni		= NULL;
	}

	~CMapBlock()
	{
		if( m_pAni )
		{
			m_pAni->Release();
			m_pAni = NULL;
		}
	};

	// ���ø�������
	void SetPos( int nPosX, int nPosY )
	{
		m_ptPos.x = nPosX;
		m_ptPos.y = nPosY;
	}

	// �õ���������
	void GetPos( int& nPosX, int& nPosY )
	{
		nPosX = m_ptPos.x;
		nPosY = m_ptPos.y;
	}

	//////////////////////////////////////////////////////////////////////////
	// ObjectManage
	// ��Ӷ�������
	void AddObject( CGameObject* pObj )
	{
		if( !pObj )	return;
		ASSERT_MSG( pObj->IsType( TypeMapobj ), _T("�Ƿ��Ķ������͡�") );
		m_ObjList.push_front( pObj->GetObjID() );
	}

	void AddObject( int nID )
	{
		CXObject* pObj = CXObjectList::GetInstance().GetObj( nID );
		if( !pObj || !pObj->IsType( TypeMapobj ) )	return;
		ASSERT_MSG( pObj->IsType( TypeMapobj ), _T("�Ƿ��Ķ������͡�") );
		m_ObjList.push_front( nID );
	}

	void DelObject( int nObjID )
	{
		CObjList::iterator iter = m_ObjList.begin();
		while( iter != m_ObjList.end() )
		{
			if( (*iter) == nObjID )
			{
				m_ObjList.erase( iter );
				break;
			}
			iter++;
		}
	}

	void DelObject( CGameObject* pObj )
	{
		DelObject( pObj->GetObjID() );
	}
	//////////////////////////////////////////////////////////////////////////
	
	void SetAnimation( CAnimation* pAni )
	{
		ASSERT_MSG( !m_pAni, _T("�Ƿ����ظ����á�") );
		if( m_pAni )	m_pAni->Release();
		m_pAni = pAni; 
		m_pAni->AddRef();
	}

};

class GAMEWORLD_API CMap	: public XObjTypeT< CXObject, TypeScene >
{
private:
	// ��ͼ
	// һ����������������еĵ�����ͼ

	CMapBlock*	m_pBlockArray;

	//typedef std::map< int, CGameObject* >	CObjMap;
	typedef std::list< CAnimation* >		CAnimationList;

	//CObjMap			m_ObjMap;			// IDӳ���,�ñ��е�ID������һ������
	CAnimationList	m_AnimationList;	// ��Ⱦ���б�

protected:
	DWORD	m_dwMapWidth;
	DWORD	m_dwMapHeight;
	SIZE	m_siBlockSize;
	int		m_nRoleID;			// ����ID, �ɱ����ƶ����ID;

public:
	typedef std::list< CMapBlock* >		CBlockList;

public:
	CMap()
	{
		// ��ͼ������Ϣ
		m_dwMapWidth			= 0;	// ��ש�������
		m_dwMapHeight			= 0;	// ��ש�������
		m_pBlockArray			= NULL;
		m_nRoleID				= INVALID_OBJID;
		m_pBlockArray			= NULL;
		ZeroMemory( &m_siBlockSize, sizeof( m_siBlockSize ) );
	}

	virtual ~CMap()
	{
		CAnimationList::iterator iter = m_AnimationList.begin();
		while( iter != m_AnimationList.end() )
		{
			int nRef = (*iter)->Release();
			ASSERT_MSG( !nRef, _T("δ�ͷŵ���Ⱦ�������ܻ�����ڴ�й¶��") );
			++iter;
		}

		if( m_pBlockArray )
		{
			delete[] m_pBlockArray;
			m_pBlockArray = NULL;
		}
	}

	// ��ʼ����ͼ
	VOID InitializeMap( DWORD dwMapWidth, DWORD dwMapHeight, int nRoleID = INVALID_OBJID );

	inline	DWORD	GetMapWidth()const		{ return m_dwMapWidth; }
	inline	DWORD	GetMapHeight()const		{ return m_dwMapHeight; }
	inline	DWORD	GetBlockWidth()const	{ return m_siBlockSize.cx; }
	inline	DWORD	GetBlockHeight()const	{ return m_siBlockSize.cy; }
	inline	DWORD	GetWorldWidth()const	{ return m_dwMapWidth* m_siBlockSize.cx; }
	inline	DWORD	GetWorldHeight()const	{ return m_dwMapHeight*m_siBlockSize.cy; }

	inline	CMapBlock*	GetBlock( DWORD x, DWORD y )const;

	// ��������ID
	inline	VOID	SetRoleID( int nID )	{ m_nRoleID = nID; }
	inline	int		GetRoleID()const		{ return m_nRoleID; }

	BOOL	GetCollisionBlock( const RECT& rc, CBlockList& retList )const;
	DWORD	Update( float fDeltaTime );

	virtual BOOL	CreateMap( LPCSTR lpszMapFilename ) = 0;

protected:
	inline	void	SetBlockSize( SIZE siBlockSize ){	m_siBlockSize = siBlockSize; }

	BOOL	PutObjToMap( int nObjID );			// ����������ͼ
	BOOL	RemoveObjFromMap( int nObjID );		// ������ӵ�ͼ�Ƴ�

	BOOL	MoveObject( CDynamic& Obj, float fDirection, float fSpeed );
	BOOL	TestCollision( CMapBlock* pBlock, const CRect& CollisionRect, int nObjID );

	void	AddAnimation( CAnimation* pAni )	{ pAni->AddRef(); m_AnimationList.push_back( pAni ); }

	// ��ɫ�ƶ�ʱ���ã�����false���ƶ�ȡ��
	virtual bool OnDynamicMove( CDynamic& pObj, float& fNewX, float& fNewY, float& fNewZ ){ return true; }

	static	bool OnEnumChild( int nID, CXObject* pObj, LPVOID pParam );
	virtual bool OnAddChild(int nID );
	virtual bool OnRemoveChild( int nID, bool bRelease );

};
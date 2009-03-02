#pragma once
#ifdef GAMEWORLD_EXPORTS
#define GAMEWORLD_API __declspec(dllexport)
#else
#define GAMEWORLD_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#include <queue>
#include "XObject.h"
#include "AttribDef.h"
#include "DataHelper.h"
#include "Animation.h"
using namespace XGC;

enum EN_MAPOBJ_ATTR
{
	Mapobj_attrVisible = 0,
	Mapobj_attrPosX,	// ��ͼ����X
	Mapobj_attrPosY,	// ��ͼ����Y
	Mapobj_attrPosZ,	// ��ͼ����Z
	Mapobj_attrScene,	// ����ID
	_Mapobj_attrCount,
};

class CMapBlock;
class CXEffect;
class CXObjectAI;
class CAction;
//////////////////////////////////////////////////////////////////////////
// ���������࣬ ������Ļ�ϵĶ��󶼴Ӵ�������
class GAMEWORLD_API CGameObject	:	public XObjTypeT< CXObject, TypeMapobj, _Mapobj_attrCount >
{
protected:
	SIZE	m_siCollision;		// ��ײ����

	CAnimation*	m_pAnimation;	// ����������, ��Ⱦ��
	CXObjectAI*	m_pAI;			// �����¼�����ӿ�
	LPVOID		m_pCustomParam;	// �û�����

	//friend class CGameObject;
	typedef std::queue< CAction* >	CActionQueue;
	CActionQueue	m_ActionQueue; // �������У�����ſ���ִ�ж������ԭ����ô���������AI�
	
	typedef std::list< int >		CEffectList;
	CEffectList		m_EffectList;	// �����б�

private:
	CRect	m_rcOld;	// ��ʱ���������������
	// CXRender	m_pRender;	// ��Ⱦ��

public:
	CGameObject();
	~CGameObject();

	inline void SetPos( float fPosX, float fPosY, float fPosZ )
	{
		SetLocalAttrib( Mapobj_attrPosX, fPosX );
		SetLocalAttrib( Mapobj_attrPosY, fPosY );
		SetLocalAttrib( Mapobj_attrPosZ, fPosZ );
	}

	// ֱ����������
	inline void GetPos( float& fPosX, float& fPosY, float& fPosZ )const
	{
		_variant_t PosX, PosY, PosZ;

		GetLocalAttrib( Mapobj_attrPosX, PosX );
		GetLocalAttrib( Mapobj_attrPosY, PosY );
		GetLocalAttrib( Mapobj_attrPosZ, PosZ );
		fPosX = V_R4(&PosX);
		fPosY = V_R4(&PosY);
		fPosZ = V_R4(&PosZ);
	}

	inline float	GetPosX()const{	_variant_t PosX; GetLocalAttrib( Mapobj_attrPosX, PosX ); return V_R4(&PosX); }
	inline float	GetPosY()const{	_variant_t PosY; GetLocalAttrib( Mapobj_attrPosY, PosY ); return V_R4(&PosY); }
	inline float	GetPosZ()const{ _variant_t PosZ; GetLocalAttrib( Mapobj_attrPosZ, PosZ ); return V_R4(&PosZ); }

	inline void		SetPosX( float fPosX )	{ SetLocalAttrib( Mapobj_attrPosX, fPosX ); }
	inline void		SetPosY( float fPosY )	{ SetLocalAttrib( Mapobj_attrPosX, fPosY ); }
	inline void		SetPosZ( float fPosZ )	{ SetLocalAttrib( Mapobj_attrPosX, fPosZ ); }

	void SetCollisionSize( int nWidth, int nHeight );
	const RECT& GetCollision()
	{
		m_rcOld.SetRect( 
			(int)( GetPosX() - m_siCollision.cx/2 ),
			(int)( GetPosZ() - m_siCollision.cy/2 ),
			(int)( GetPosX() - m_siCollision.cx/2 + m_siCollision.cx - 1 ),
			(int)( GetPosZ() - m_siCollision.cy/2 + m_siCollision.cy - 1 ) );
		return m_rcOld;
	}


	void		AttachAI( CXObjectAI* pAI ){ m_pAI = pAI; }
	CXObjectAI*	DetachAI(){ CXObjectAI* pTmpAI = m_pAI; m_pAI = NULL; return pTmpAI; }
	CXObjectAI*	GetObjectAI(){ return m_pAI; }
	BOOL		SetMap( CMap* pMap );

	BOOL	Think( int nObjID, int nEvent, LPCSTR lpBuf, size_t nSize );
	//////////////////////////////////////////////////////////////////////////
	// return �Ƿ�������ж���	[TRUE - ȫ�����] [FALSE - ���ж�����Ҫ�ȴ�]
	virtual BOOL DoAction( const CMap& Map );

	CAnimation* GetAnimation()const	{ return m_pAnimation; }
	void		SetAnimation( CAnimation* pAni ) { m_pAnimation = pAni; m_pAnimation->AddRef(); }

	/************************************************************************/
	/* �û���������                                                         */
	/************************************************************************/
	void		SetCustomParam( LPVOID pParam ) { m_pCustomParam = pParam; }
	LPVOID		GetCustomParam() const			{ return m_pCustomParam; }

	/************************************************************************/
	/* �����б����
	/************************************************************************/
	void		RemoveEffect( int nObjID );
	void		AddEffect( int nObjID );
	void		AddEffect( CXEffect* pEffect );
	int			MoveEffectTo( CGameObject* pObj );

	// �����߼�
	// Map			:	��ͼ���ݶ��������
	// fDeltaTime	:	ʱ���
	virtual	DWORD	Update( const CMap& Map, float fDeltaTime );
};

//////////////////////////////////////////////////////////////////////////
// BombMan ����
enum EN_DYNAMIC_ATTR
{
	Dynamic_attrSpeed = 0,
	Dyanmic_attrHP,
	Dyanmic_attrHPMAX,
	Dynamic_attrDirection,
	Dynamic_attrMoveState,		// ��ɫ�ƶ�״̬
	_Dynamic_attrCount,
};

#define DynamicStand	0
#define DynamicMove		1

class GAMEWORLD_API CDynamic :	public	XObjTypeT< CGameObject, TypeDynamic, _Dynamic_attrCount >
{
public:
	CDynamic();
	virtual ~CDynamic(void);

	void	Run( bool bRun )			{ SetLocalAttrib( Dynamic_attrMoveState, bRun ); }
	void	SetSpeed( float fSpeed )	{ SetLocalAttrib( Dynamic_attrSpeed, fSpeed ); }
	float	GetSpeed()					{ _variant_t Speed( 0.0f ); GetLocalAttrib( Dynamic_attrSpeed, Speed ); return V_R4(&Speed); }

	void	SetDirection( float fAngle ){ SetLocalAttrib( Dynamic_attrDirection, fAngle ); }
	float	GetDirection()				{ _variant_t Angle( 0.0f ); GetLocalAttrib( Dynamic_attrDirection, Angle ); return V_R4(&Angle); }
};
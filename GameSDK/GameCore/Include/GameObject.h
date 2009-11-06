#pragma once
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#include <queue>
#include "Loki/Function.h"

class CMapBlock;
class CGameMap;
class CXObjectAI;
//////////////////////////////////////////////////////////////////////////
// ���������࣬ ������Ļ�ϵĶ��󶼴Ӵ�������
class CORE_API CGameObject	:	public XObjOnlyTypeT< CGameObject, CXObject, TypeGameObject >
{
friend class CGameMap;
public:
	CGameObject();
	~CGameObject();

	inline void SetPosition( float fPosX, float fPosY, float fPosZ )
	{
		m_Position.SetValue( fPosX, fPosY, fPosZ );
	}

	inline void SetPosition( const float fPos[3] )
	{
		m_Position[0] = fPos[0];
		m_Position[1] = fPos[1];
		m_Position[2] = fPos[2];
	}

	// ֱ����������
	inline void GetPosition( float fPos[3] )const
	{
		fPos[0] = m_Position[0];
		fPos[1] = m_Position[1];
		fPos[2] = m_Position[2];
	}

	// ֱ����������
	inline void GetPosition( XVector3& Position )const
	{
		Position = m_Position;
	}

	// ֱ����������
	inline const XVector3& GetPosition()const
	{
		return m_Position;
	}

	inline float GetPosX()const{ return m_Position.x; }
	inline float GetPosY()const{ return m_Position.y; }
	inline float GetPosZ()const{ return m_Position.z; }

	inline float GetWidth()const{ return 1.0f; }
	inline float GetHeight()const{ return 1.0f; }
	DEBUG_CODE
	(
	void SetBlock( void* pBlock ){ m_pBlock = pBlock; }
	void* GetBlock()const{ return m_pBlock; }
	)
private:
	XVector3	m_Position;
	DEBUG_CODE( void *m_pBlock; )
};

class CORE_API CDynamicObject :	public	XObjTypeT< CDynamicObject, CGameObject, TypeDynamicObject, _DynamicObject_attrCount >
{
public:
	CDynamicObject();
	virtual ~CDynamicObject(void);

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	���������Ұ
	// pObject	:	������Ұ�Ķ���ָ��
	//---------------------------------------------------//
	virtual void EnterEyeshot( CDynamicObject* pObject ){}

	//---------------------------------------------------//
	// [8/3/2009 Albert]
	// Description:	�����뿪��Ұ
	// pObject	:	�뿪��Ұ�Ķ���ָ��
	//---------------------------------------------------//
	virtual void LeaveEyeshot( CDynamicObject* pObject ){}

	//---------------------------------------------------//
	// [9/9/2009 Albert]
	// Description:	����Ŀ��ʱ����
	//---------------------------------------------------//
	virtual void OnArrived( const XVector3& vPosition ){}

	//---------------------------------------------------//
	// [9/10/2009 Albert]
	// Description:	ÿ���ƶ������
	//---------------------------------------------------//
	virtual void OnStep( const XVector3& vPosition ){}

	//---------------------------------------------------//
	// [8/4/2009 Albert]
	// Description:	���·����
	//---------------------------------------------------//
	__inline void BeginPathPoint();

	//---------------------------------------------------//
	// [8/4/2009 Albert]
	// Description:	���·��
	//---------------------------------------------------//
	__inline void AddPathPoint( XVector3 vPosition, float fSpeed );

	//---------------------------------------------------//
	// [8/5/2009 Albert]
	// Description:	���·�����
	//---------------------------------------------------//
	__inline void FinishPathPoint( float fDelayTime = 0.0f );

	//---------------------------------------------------//
	// [9/9/2009 Albert]
	// Description:	�ƶ���ָ��λ��
	//---------------------------------------------------//
	void MoveTo( XVector3 vPosition, float fSpeed, float fDelayTime = 0.0f );

	//---------------------------------------------------//
	// [9/9/2009 Albert]
	// Description:	�����ƶ��ٶ�
	//---------------------------------------------------//
	__inline void SetSpeed( float fSpeed )	{ SetLocalAttrib( DynamicObject_attrSpeed, fSpeed ); }

	//---------------------------------------------------//
	// [9/9/2009 Albert]
	// Description:	��ȡ�ƶ��ٶ�
	//---------------------------------------------------//
	__inline float GetSpeed()const { return GetLocalAttrib( DynamicObject_attrSpeed ); }

	//---------------------------------------------------//
	// [9/10/2009 Albert]
	// Description:	�Ƿ��ڸ�����
	//---------------------------------------------------//
	__inline bool IsUpdate()const{ return m_hUpdateTimer != INVALID_TIMER_HANDLE; }

	//---------------------------------------------------//
	// [9/16/2009 Albert]
	// Description:	���ø��º���
	//---------------------------------------------------//
	__inline bool SetUpdate( TimerFunction &Fn, unsigned short repeat = 1, unsigned int timer = TIMER_SECONDS(1), unsigned int delay = 0 );

	//---------------------------------------------------//
	// [9/16/2009 Albert]
	// Description:	ֹͣ����
	//---------------------------------------------------//
	__inline void StopUpdate();

	//---------------------------------------------------//
	// [9/16/2009 Albert]
	// Description:	��ȡ��һ���ƶ���
	//---------------------------------------------------//
	__inline XVector3& GetNextPosition(){ return m_vNextPosition; }

	//---------------------------------------------------//
	// [9/18/2009 Albert]
	// Description:	���õ�ǰ����
	//---------------------------------------------------//
	__inline XVector3& ResetPosition(){ m_vNextPosition = GetPosition(); return m_vNextPosition; }
protected:
	//---------------------------------------------------//
	// [8/5/2009 Albert]
	// Description:	��ʱ�����øú���,���ڵ�������ʵ���ƶ�
	//---------------------------------------------------//
	bool UpdateTimer( unsigned int /*handle*/, unsigned short& /*repeat*/, unsigned int& /*timer*/ );

private:
	struct PathPoint
	{
		XVector3	vPosition;	// ·��λ��
		float		fSpeed;		// �õ�ʹ�õ��ٶ�
	};
	typedef std::list< PathPoint, ALLOCATOR< PathPoint > >	CPointList;

	CPointList	m_PointList;		// ·���б�
	XVector3	m_vNextPosition;	// ��һ��Ŀ���
	_uint32		m_hUpdateTimer;		// λ�ƶ�ʱ�����
};
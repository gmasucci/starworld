#include "StdAfx.h"
#include "GameObject.h"
#include "GameMap.h"
BEGIN_ATTRIBUTE_TABLE(CDynamicObject, CGameObject)
	DEFINE_ATTRIBUTE( _T("MoveFlag"), DynamicObject_attrCanMove, 0 )
END_ATTRIBUTE_TABLE()

IMPLEMENT_ATTRIBUTE(CDynamicObject, CGameObject)

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

CDynamicObject::CDynamicObject()
: m_hUpdateTimer( INVALID_TIMER_HANDLE )
{

}

CDynamicObject::~CDynamicObject(void)
{
	if( m_hUpdateTimer != INVALID_TIMER_HANDLE )
	{
		ThisTimer()->remove_event( m_hUpdateTimer );
		m_hUpdateTimer = INVALID_TIMER_HANDLE;
	}
}

//---------------------------------------------------//
// [8/4/2009 Albert]
// Description:	���·����
//---------------------------------------------------//
void CDynamicObject::BeginPathPoint()
{
	if( m_hUpdateTimer != INVALID_TIMER_HANDLE )
	{
		unsigned int release_time = ThisTimer()->remove_event( m_hUpdateTimer );
		m_hUpdateTimer = INVALID_TIMER_HANDLE;

		if( release_time > 0 )
		{
			// ��������
			XVector3 Vec = m_vNextPosition - GetPosition();
			Vec.NormalizeFast();
			// ʣ���˶��پ���
			Vec *= release_time/1000.0f;
			// �´˸��������ʣ�����Ĳ�ֵ��Ϊ��ǰ��λ��
			m_vNextPosition -= Vec;

			CGameMap* pScene = static_cast< CGameMap* >( CXObjectPool::GetInstance().GetObj( GetParent(), TypeGameMap ) );
			if( pScene )
			{
				if( pScene->DynamicMoveTo( this, m_vNextPosition, false ) )
				{
					OnStep( m_vNextPosition );
				}
			}
		}
	}
	m_PointList.clear();
}

//---------------------------------------------------//
// [8/4/2009 Albert]
// Description:	���·��
//---------------------------------------------------//
void CDynamicObject::AddPathPoint( XVector3 vPosition, float fSpeed )
{
	vPosition.y = 0.0f;
	PathPoint p = { vPosition, fSpeed };
	m_PointList.push_back( p );
}

//---------------------------------------------------//
// [8/4/2009 Albert]
// Description:	������·��
//---------------------------------------------------//
void CDynamicObject::FinishPathPoint( float fDelayTime )
{
	// timer ����Ϊ -1 ���ʾ������ִ��
	SetUpdate(
		_tfunction( bind( &CDynamicObject::UpdateTimer, this, _1, _2, _3 ) ),
		1, 
		TIMER_SECONDS( 1.0f ), 
		TIMER_SECONDS( fDelayTime ) );
}

//---------------------------------------------------//
// [9/9/2009 Albert]
// Description:	�ƶ���ָ��λ��
//---------------------------------------------------//
void CDynamicObject::MoveTo( XVector3 vPosition, float fSpeed, float fDelayTime )
{
	if( m_hUpdateTimer != INVALID_TIMER_HANDLE )
	{
		ThisTimer()->remove_event( m_hUpdateTimer );
		m_hUpdateTimer = INVALID_TIMER_HANDLE;
	}
	m_PointList.clear();

	PathPoint p = { vPosition, fSpeed };
	m_PointList.push_back( p );

	SetUpdate(
		_tfunction( bind( &CDynamicObject::UpdateTimer, this, _1, _2, _3 ) ),
		1, 
		TIMER_SECONDS( 1.0f ), 
		TIMER_SECONDS( fDelayTime ) );
}

//---------------------------------------------------//
// [8/5/2009 Albert]
// Description:	�ƶ���ʱ��
//---------------------------------------------------//
bool CDynamicObject::UpdateTimer( unsigned int handle, unsigned short& repeat, unsigned int& timer )
{
	repeat = 1;
	timer = TIMER_SECONDS(1.0f);

	// ���ƶ�����ǰ���ڵ�λ��.
	CGameMap* pScene = static_cast< CGameMap* >( CXObjectPool::GetInstance().GetObj( GetParent(), TypeGameMap ) );
	if( pScene )
	{
		if( pScene->DynamicMoveTo( this, m_vNextPosition, false ) )
		{
			//CLogger::GetInstance(_T("Log"))->WriteLog( _T("��ɫ[%08x]�ƶ���·��[%f,%f,%f]")
			//	, GetObjID()
			//	, m_vNextPosition[0], m_vNextPosition[1], m_vNextPosition[2] );
			OnStep( m_vNextPosition );
		}
		else
		{
			OnArrived( m_vNextPosition );
			return true;
		}
	}
	else
	{
		m_hUpdateTimer = INVALID_TIMER_HANDLE;
		return true;
	}

	// ������һ����
	if( m_PointList.empty() ) 
	{
		m_hUpdateTimer = INVALID_TIMER_HANDLE;
		OnArrived( m_vNextPosition );
		return true;
	}

	const PathPoint& target = m_PointList.front();

	XVector3 Vec = target.vPosition - m_vNextPosition;

	float distance = Vec.SqurLength();
	float fSpeed = target.fSpeed;
	if( distance < fSpeed*fSpeed )
	{
		// ��ǰ���굽Ŀ���ľ���С���ٶ�ֵ
		m_vNextPosition = target.vPosition;

		float t = XMath::Sqrt( distance )/fSpeed;
		timer = TIMER_SECONDS(t);
		m_PointList.pop_front();
	}
	else
	{
		Vec.NormalizeFast();
		Vec *= fSpeed;
		m_vNextPosition += Vec;
	}

	return false;
}


//---------------------------------------------------//
// [9/16/2009 Albert]
// Description:	���ø��º���
//---------------------------------------------------//
bool CDynamicObject::SetUpdate( _tfunction &Fn, unsigned short repeat, unsigned int timer, unsigned int delay )
{
	StopUpdate();
	ResetPosition();
	// delay ����Ϊ -1 ���ʾ������ִ��
	m_hUpdateTimer = ThisTimer()->insert_event( Fn, repeat, timer, delay );

	return m_hUpdateTimer != INVALID_TIMER_HANDLE;
}

//---------------------------------------------------//
// [9/16/2009 Albert]
// Description:	ֹͣ����
//---------------------------------------------------//
void CDynamicObject::StopUpdate()
{
	if( m_hUpdateTimer != INVALID_TIMER_HANDLE )
	{
		ThisTimer()->remove_event( m_hUpdateTimer );
		m_hUpdateTimer = INVALID_TIMER_HANDLE;
	}
}
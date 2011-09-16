#pragma once
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

namespace XGC
{
	class CGameObject;
	class CORE_API CObjectControlerBase
	{
	public:
		CObjectControlerBase();
		CObjectControlerBase( CGameObject *pObject );
		virtual ~CObjectControlerBase(void);

		//---------------------------------------------------//
		// [11/24/2010 Albert]
		// Description:	���ӿ��ƿ��ƶ� 
		//---------------------------------------------------//
		void Attach( CGameObject* pObject );

		//---------------------------------------------------//
		// [11/24/2010 Albert]
		// Description:	�Ͽ����ƶ�
		//---------------------------------------------------//
		CGameObject* Detach();

		//---------------------------------------------------//
		// [12/1/2010 Albert]
		// Description:	��ͣ�����߼� 
		//---------------------------------------------------//
		void PauseUpdate();

		//---------------------------------------------------//
		// [12/1/2010 Albert]
		// Description:	�ָ������߼� 
		//---------------------------------------------------//
		void ResumeUpdate();

		//---------------------------------------------------//
		// [11/24/2010 Albert]
		// Description:	���ø��� 
		//---------------------------------------------------//
		void SetUpdate( const _tfunction &fn, int nRepeat, int nInterval, int nDelay );

		//---------------------------------------------------//
		// [9/10/2009 Albert]
		// Description:	�Ƿ��ڸ�����
		//---------------------------------------------------//
		__inline bool IsUpdate()const{ return m_hUpdateTimer != INVALID_TIMER_HANDLE; }

		//---------------------------------------------------//
		// [9/16/2009 Albert]
		// Description:	ֹͣ����
		//---------------------------------------------------//
		unsigned int StopUpdate();

	private:
		virtual void OnStopUpdate( unsigned int nReleaseTime ){};

	protected:
		CGameObject*	m_pHost;
		timer_t			m_hUpdateTimer;		// ��ʱ�����
	};

	class CORE_API CObjectMoveControler	:	public CObjectControlerBase
	{
	public:
		CObjectMoveControler( CGameObject* pObject = xgc_nullptr, bool bCollision = false );
		~CObjectMoveControler();

		enum { MoveBegin, MoveArrived, MoveStop, MoveStep, };

		//---------------------------------------------------//
		// [12/16/2010 Albert]
		// Description:	�ƶ���Ŀ��λ�� 
		//---------------------------------------------------//
		void MoveTo( XVector3 vPosition, float fSpeed, float fDelay = 0.0f );

		//---------------------------------------------------//
		// [8/4/2009 Albert]
		// Description:	���·����
		//---------------------------------------------------//
		virtual void OnStopUpdate( unsigned int nReleaseTime );

		//---------------------------------------------------//
		// [12/10/2010 Albert]
		// Description:	��ȡ�ƶ��ٶ� 
		//---------------------------------------------------//
		float GetSpeed()const{ return m_fSpeed; }

		//---------------------------------------------------//
		// [12/10/2010 Albert]
		// Description:	��ȡ�ٶ����� 
		//---------------------------------------------------//
		const XVector3& GetVectorSpeed()const{ return m_vSpeed; }

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	��ȡĿ��� 
		//---------------------------------------------------//
		void GetTargetPosition( float vTargetPosition[3] )const
		{ 
			vTargetPosition[0] = m_vTargetPosition[0]; 
			vTargetPosition[1] = m_vTargetPosition[1]; 
			vTargetPosition[2] = m_vTargetPosition[2]; 
		}

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	��ȡ��һ���ƶ��� 
		//---------------------------------------------------//
		void GetNextPosition( float vNextPosition[3] )const
		{ 
			vNextPosition[0] = m_vNextPosition[0]; 
			vNextPosition[1] = m_vNextPosition[1]; 
			vNextPosition[2] = m_vNextPosition[2]; 
		}

	protected:
		//---------------------------------------------------//
		// [8/5/2009 Albert]
		// Description:	��ʱ�����øú���,���ڵ�������ʵ���ƶ�
		//---------------------------------------------------//
		bool UpdatePosition( unsigned int /*handle*/ );

		//---------------------------------------------------//
		// [11/22/2010 Albert]
		// Description:	������һ�θ��µ������ 
		//---------------------------------------------------//
		float CalcNextPosition();
	private:
		XVector3	m_vNextPosition;	// ��ǰ��
		XVector3	m_vTargetPosition;	// Ŀ���
		XVector3	m_vSpeed;
		float		m_fSpeed;
		bool		m_bCollision;
	};

	class CORE_API CObjectIdleControler	:	public CObjectControlerBase
	{
	public:
		CObjectIdleControler( CGameObject* pObject = xgc_nullptr );
		~CObjectIdleControler();

		enum { IdleTimeout, };
		void Idle( float fTime = 0.0f );

	protected:
		//---------------------------------------------------//
		// [8/5/2009 Albert]
		// Description:	��ʱ�����øú���,���ڵ�������ʵ���ƶ�
		//---------------------------------------------------//
		bool UpdateIdle( unsigned int handle );

	private:
		float		m_fIdleTime;
	};

}

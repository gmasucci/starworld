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
	enum eStatus { status_append, status_active, status_remove, timing_count };

	// ����ָ������ָ��
	class CORE_API CXAction;
	typedef int (*ActionFunc)( xObject hSource, xObject hDest, const char* pBuf, unsigned int nSize );
	typedef int (*StatusFunc)( xObject hDest, int nFlag, long_ptr& pUserdata, const char* pBuf, unsigned int nSize );

	class CORE_API CXAction	:	public CXObject
	{
		DECLARE_DYNAMICTYPE( CXObject, TypeAction );
	public:
		CXAction( const ActionFunc Functor, cbuffer Params, float fDelay );
		~CXAction();

		//---------------------------------------------------//
		// [11/18/2010 Albert]
		// Description:	����� 
		//---------------------------------------------------//
		int ActiveAction( xObject hSource, xObject hTarget );

		//---------------------------------------------------//
		// [11/26/2010 Albert]
		// Description:	��ȡ�������� 
		//---------------------------------------------------//
		cbuffer GetParams()const{ return m_Params; }

		//---------------------------------------------------//
		// [11/26/2010 Albert]
		// Description:	��ȡ�ӳ�ʱ�� 
		//---------------------------------------------------//
		float GetDelay()const{ return m_fDelay; }

	protected:
		//---------------------------------------------------//
		// [11/18/2010 Albert]
		// Description:	ִ�ж��� 
		//---------------------------------------------------//
		int DoAction( xObject hSource, xObject hTarget );

	private:
		ActionFunc	m_Functor;
		cbuffer		m_Params;
		float		m_fDelay;
	};

	class CORE_API CXStatusInstance
	{
	public:
		CXStatusInstance();
		~CXStatusInstance();

		//---------------------------------------------------//
		// [12/14/2010 Albert]
		// Description:	������� 
		//---------------------------------------------------//
		void AppendEffect( const StatusFunc& Functor, cbuffer Params );

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	ִ��״̬ 
		//---------------------------------------------------//
		void DoStatus( xObject nIdentity, long_ptr& pUserdata, eStatus eFlag );
	private:
		struct effect
		{
			cbuffer		mParams;
			StatusFunc	mFunctor;
		};

		typedef xgc_vector< effect > EffectContainer;

		EffectContainer	m_EffectContainer;
	};

	class CORE_API CXStatus : public CXObject
	{
		DECLARE_DYNAMICTYPE( CXObject, TypeStatus );
	public:
		CXStatus( CXStatusInstance* pInstance )
			: m_pInstance( pInstance )
			, m_hUpdateTimerHandler( INVALID_TIMER_HANDLE )
		{
		}

		~CXStatus();

		//---------------------------------------------------//
		// [12/14/2010 Albert]
		// Description:	����״̬ 
		//---------------------------------------------------//
		bool OnUpdate( timer_t handle );

		//---------------------------------------------------//
		// [12/15/2010 Albert]
		// Description:	״̬���� 
		//---------------------------------------------------//
		bool OnStatusOver( timer_t handle );

		//---------------------------------------------------//
		// [9/13/2009 Albert]
		// Description:	��װ��ʱ��
		//---------------------------------------------------//
		void InstallTimer( float fLifetime, float fInterval, float fDelay = 0 );

		//---------------------------------------------------//
		// [11/19/2010 Albert]
		// Description:	ִ��״̬ 
		//---------------------------------------------------//
		__inline void DoStatus( eStatus eFlag )
		{
			m_pInstance->DoStatus( GetParent(), m_Userdata, eFlag );
		}

		virtual void OnDestroy()
		{
			m_pInstance->DoStatus( GetParent(), m_Userdata, status_remove );
		}
	private:
		CXStatusInstance* m_pInstance;
		timer_t m_hUpdateTimerHandler;
		timer_t m_hLifetimeHandler;

		union
		{
			unsigned int u;
			int n;
			float f;
			long_ptr m_Userdata;
		};
	};
}
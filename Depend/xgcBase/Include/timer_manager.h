#pragma once
//////////////////////////////////////////////////////////////////////////
// Loki ͷ�ļ�

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif
#ifndef _USE_BOOST
	#include <functional>
	using namespace std;
	using namespace std::tr1;
	using namespace std::tr1::placeholders;
#else
	#include "boost\function.hpp"
	#include "boost\bind.hpp"
	using namespace boost;
#endif
namespace XGC
{
	namespace common
	{
		//////////////////////////////////////////////////////////////////////////
		// ��ʱ��������
		// base		:	��׼ʱ��
		//////////////////////////////////////////////////////////////////////////
		typedef unsigned int timer_t;
		class EXPORT_LIB timer
		{
		public:
			timer(void);
			~timer(void);

		public:
			void active();
			//////////////////////////////////////////////////////////////////////////
			// ��ʱ���¼��ӿ�
			//////////////////////////////////////////////////////////////////////////
			typedef function< bool ( unsigned int ) > _efunction;
			struct _tevent	/*:	public CPoolAllocator*/
			{
				friend class timer;
			protected:
				//------------------------------------------------//
				// [7/28/2008] Write by Albert.xu
				// Description		: ����ʱ���¼�����
				// _function	: ����ָ���������operator()�Ľṹ��
				// _repeat		: �ظ�����, ����ִ��һ��.
				// _timer		: �¼��Ĵ���ʱ����
				//------------------------------------------------//
				_tevent( _efunction &_function, unsigned int _repeat, unsigned int _timer, uintptr_t _user_ptr = 0 )
					: function( _function )
					, handle(-1)
					, repeat(_repeat)
					, interval(_timer)
					, index(0)
					, user_ptr( _user_ptr )
				{

				};

				virtual ~_tevent()
				{
					function = NULL;
					handle	= -1;
					repeat	= 0;
					interval= 0;
					index	= 0;
					user_ptr = 0;
				}

			public:
				__inline uintptr_t&		get_user_ptr()		{ return user_ptr; }
				__inline timer_t		get_handle()const	{ return handle; } 
				__inline unsigned int	get_invent()const	{ return interval; } 
				__inline unsigned int	get_repeat()const	{ return repeat; }

				__inline void set_interval( unsigned int _interval ){ interval = _interval; } 
				__inline void set_repeat( unsigned int _repeat ){ repeat = _repeat; }

			private:
				void release(){ delete this; }

				_efunction		function;
				timer_t			handle;	// ���
				unsigned int	interval;	// ʱ����, ��׼֮��ı���
				unsigned int	repeat;	// ִ�д���, ִ�д���Ϊ��Ϊ0 ������
				unsigned int	index;	// ������, �������Ǹ�������
				uintptr_t		user_ptr;	// �û�����

				size_t	round;	// ѭ�����,��ź�TimerManager��ѭ��������ʱ,�ö���ű�ִ��.
			};

		public:
			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	��ʼ��ʱ���¼�������
			// length	:	������С,��ֵԽ��Ч��Խ��
			// return	:
			//------------------------------------------------//
			bool initialize( unsigned int length, unsigned int max_event_object );

			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	�����¼�
			// event	:	�¼�����
			// return	:	�¼����
			//------------------------------------------------//
			__inline timer_t insert_event( _tevent* event, unsigned int delay = 0 );

			//---------------------------------------------//
			// [11/21/2008 write by albert]
			// ������;	:	insert event
			// function	:	�ص�����,�����ǳ�Ա����,�º�������ָ��
			// repeat	:	�ظ�����
			// timer	:	���ʱ��
			// return	:	��ʱ�����
			//---------------------------------------------//
			timer_t insert_event( _efunction function, unsigned int repeat, unsigned int interval, unsigned int delay = 0 );

			//---------------------------------------------------//
			// [9/17/2009 Albert]
			// Description:	�ӳٴ���ʱ���¼�
			// handle	:	�¼����
			// delay	:	�ӳٶ��ٵδ�
			//---------------------------------------------------//
			bool delay_event( unsigned int handle, unsigned int delay );

			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	�õ��¼�����
			// handle	:	�¼����
			// return	:	�¼�����,δ�ҵ�����NULL
			//------------------------------------------------//
			_tevent* get_event( unsigned int handle );

			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	ɾ���¼�
			// event	:	�¼�����
			//------------------------------------------------//
			unsigned int remove_event( _tevent* event );

			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	ɾ���¼�
			// handle	:	�¼����
			//------------------------------------------------//
			__inline unsigned int remove_event( timer_t handle );

			//---------------------------------------------------//
			// [11/29/2010 Albert]
			// Description:	��ͣ���� 
			//---------------------------------------------------//
			void pause_event( timer_t handle );

			//---------------------------------------------------//
			// [11/29/2010 Albert]
			// Description:	�ָ����� 
			//---------------------------------------------------//
			void resume_event( timer_t handle );
		protected:
			//------------------------------------------------//
			// [7/24/2008] Write by Albert.xu
			// Description	:	������󵽶�ʱ���б���
			//	handle	:	ʱ���¼�������
			//------------------------------------------------//
			void inner_insert( timer_t handle, unsigned int delay = 0 );

		private:
			struct Impl;
			Impl* m_Impl;

			struct timer_tandle
			{
				unsigned int	position:24;	// λ��λ��
				unsigned int	round:8;		// ��ѯλ��
			};

			// ��ʱ������
			size_t	m_next_list;	// ��һ���б�
			size_t	m_timer_round;	// ��һ��ѭ��

			// ����ر���
			size_t	m_object_count;	// �¼��������
			size_t	m_cur_pos;		// ��ǰ���еĵ�Ԫ
			size_t	m_round;		// ѭ������
		};

		#define INVALID_TIMER_HANDLE (-1)
		#define TIMER_IMMEDIATELY (0)
		typedef timer::_tevent		_tevent;
		typedef timer::_efunction	_tfunction;
		// #include "TimerManager.inl"
	}
}
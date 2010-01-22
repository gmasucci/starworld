#pragma once

template< class E >
class CState
{
public:
	CState(void);
	virtual ~CState(void);
	//--------------------------------------------------------//
	//	created:	21:1:2010   14:33
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬ǰ����
	//	pEntity:	����״̬�Ķ���ָ��
	//	return:		TRUE ����״̬�ɹ��� false����״̬ʧ��
	//--------------------------------------------------------//
	virtual bool OnEnter( E *pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:34
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬
	//	pEntity:	����״̬�Ķ���ָ��
	//--------------------------------------------------------//
	virtual void OnUpdate( E *pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�ж�
	//--------------------------------------------------------//
	virtual void OnInterrupt( E* pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϻָ��ֳ�
	//--------------------------------------------------------//
	virtual void OnIret( E* pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:35
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�뿪״̬
	//	pEntity:	����״̬�Ķ���ָ��
	//--------------------------------------------------------//
	virtual void OnLeave( E *pEntity ) = 0;
};

template< class E, int C = 8 >
class CStateTemplate
{
public:
	typedef CState< E >			_ESTATE;	// ״̬
	typedef CStateTemplate< E >	_THIS;

	typedef std::stack< _uint8 >			_STATESTACK; // ״̬����
	typedef Loki::Function< _uint8 ( E* ) >	_GUARDFUNC;// �ػ�����
	typedef std::list< _GUARDFUNC >			_GUARDLIST; // �ػ������б�

public:
	CStateTemplate( E *pOwner )
		: m_pOwner( pOwner )
		, m_nCurIndex( 0 )
	{
		memset( m_StatePool, 0, sizeof(m_StatePool) );
	}

	~CStateTemplate()
	{

	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬ӳ���
	//--------------------------------------------------------//
	bool MapState( _uint8 nIndex, _ESTATE *pState )
	{
		if( nIndex >= _countof(m_StatePool) ) 
			return false;

		m_StatePool[nIndex].state = pState;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	ע��״̬Ǩ��·�����ػ�����
	//--------------------------------------------------------//
	bool RegistStateGuard( _uint8 nIndex, _GUARDFUNC& Func )
	{
		if( nIndex >= _countof(m_StatePool) ) 
			return false;

		_STATE& State = m_StatePool[nIndex];
		if( !State.state )
			return false;

		State.guards.push_back( Func );
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬��
	//--------------------------------------------------------//
	void Update()
	{
		_STATE& State = m_StatePool[m_nCurIndex];
		if( State.state )
		{
			State.state->OnUpdate( m_pOwner );
			_GUARDLIST::iterator i = State.guards.begin();
			while( i != State.guards.end() )
			{
				_uint8 nNewState = (*i)( m_pOwner );
				if( nNewState != -1 )
				{
					ChangeTo( nNewState );
					break;
				}
				++i;
			}
		}
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	������Ǩ��״̬
	//--------------------------------------------------------//
	bool ChangeTo( _uint8 nIndex )
	{
		if( nIndex >= _countof(m_StatePool) )
			return false;

		_STATE& _OldState = m_StatePool[m_nCurIndex];
		_STATE& _NewState = m_StatePool[nIndex];
		if( _NewState.state == NULL )
			return false;

		if( _NewState.state->OnEnter( m_pOwner ) == false )
			return false;

		if( _OldState.state )
			_OldState.state->OnLeave( m_pOwner );

		m_nCurIndex = nIndex;
		return true;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:11
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϵ�ǰ״̬�����жϵ�״̬���Զ�����
	//				��ǰ״̬�������ģ��ȵ��ж��˳���ʱ�򱻻ָ���
	//--------------------------------------------------------//
	bool Interrupt( _uint8 nIndex )
	{
		m_ss.push( nIndex );

		if( nIndex >= _countof(m_StatePool) )
			return false;

		_STATE& _OldState = m_StatePool[m_nCurIndex];
		_STATE& _NewState = m_StatePool[nIndex];

		if( _NewState.state == NULL )
			return false;

		if( _NewState.state->OnEnter( m_pOwner ) == false )
			return false;

		if( _OldState.state )
			_OldState.state->OnInterrupt( m_pOwner );

		m_nCurIndex = nIndex;
		return true;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:14
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϷ��أ����жϵ�״̬Ҫ�����жϷ��غ����л���ԭ�ȵ�״̬��
	//--------------------------------------------------------//
	bool Iret()
	{
		_uint8 nIndex = m_ss.top();
		m_ss.pop();

		if( nIndex >= _countof(m_StatePool) )
			return false;

		_STATE& _OldState = m_StatePool[m_nCurIndex];
		_STATE& _NewState = m_StatePool[nIndex];

		if( _NewState.state == NULL )
			return false;

		if( _NewState.state->OnIret( m_pOwner ) == false )
			return false;

		if( _OldState.state )
			_OldState.state->OnLeave( m_pOwner );

		m_nCurIndex = nIndex;

		return true;
	}

private:
	typedef struct _STATE_
	{
		_ESTATE*	state;
		_GUARDLIST	guards;
	}_STATE, *_PSTATE;

	E*		m_pOwner;
	_STATE	m_StatePool[C];
	_GUARDLIST	m_gl;
	_STATESTACK	m_ss;

	_uint8	m_nCurIndex;
};

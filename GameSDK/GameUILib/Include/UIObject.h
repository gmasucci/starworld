#ifndef _UIOBJECT_
#define _UIOBJECT_
/************************************************************************/
/* �б�ϵͳ������ϵͳ���Լ�����������ʵ��                               */
/************************************************************************/
#define INVALID_OBJID -1
#define INVALID_ATTRIBID -1
#define OBJ_MAX	65535

#define		TypeObjectbase					0

#define		TypeUI							0x30000000
#define			TypeElement					0x31000000
#define				TypeButton				0x31100000
#define				TypeEditBox				0x31200000
#define				TypeScroll				0x31300000
#define				TypeProgress			0x31400000
#define				TypePicture				0x31500000
#define					TypeMultiPicture	0x31510000
#define					TypeRenderTarget	0x31520000
#define					TypeStatic			0x31530000
#define					TypeWindow			0x31540000
#define						TypeDialog		0x31551000
#define					TypeListbox			0x31560000

#include <Windows.h>

/************************************************************************/
/* CXObjectT ģ��ʵ���˶�̬����ϵͳ��
/************************************************************************/
template < typename TBase, int TYPE >
class UIObjTypeT	:	public TBase
{
protected:
	UIObjTypeT()
		:TBase()
	{
		//ASSERT( IsTypeOf( TypeObjectbase ), "����ļ̳����͡�" );
	}

public:
	static	int		GetPointType()					{ return TYPE; }
	virtual int		GetType()const					{ return TYPE; }
	virtual bool	IsKindOf( unsigned int nType )const	{ if( this == NULL ) return false;if( GetPointType() == nType ) return true; else return TBase::IsKindOf( nType );}
	static	bool	IsTypeOf( unsigned int nType )		{ if( GetPointType() == nType ) return true; else return TBase::IsTypeOf( nType );}
};

/*	:	public XObjTypeT< CXObjectBase, EN_OBJECT, 1 > */
class CUIObject
{
protected:
	CUIObject();
	virtual ~CUIObject();

public:
	static	int		GetPointType()					{ return TypeObjectbase;		}
	virtual int		GetType()const					{ return TypeObjectbase;		}
	virtual bool	IsKindOf( unsigned int nType )const	{ return TypeObjectbase == nType;}
	static	bool	IsTypeOf( unsigned int nType )		{ return TypeObjectbase == nType;}
};
#endif //_UIOBJECT_
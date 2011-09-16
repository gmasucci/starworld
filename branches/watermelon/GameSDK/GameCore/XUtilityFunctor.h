#pragma once
#ifndef _XUTILITY_FUNCTOR_H
#define _XUTILITY_FUNCTOR_H
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS

#include "XObject.h"
#include "XFsmBase.h"

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	�ж��Ƿ�Ϊ��Ч�Ķ��� 
//---------------------------------------------------//
CORE_API bool CheckObjectType( xObject hObject, xType TypeId );

//---------------------------------------------------//
// [11/16/2010 Albert]
// Description:	��ȡ���Բ����� 
//---------------------------------------------------//
CORE_API CAttribute GetAttribute( xObject hObject, xType gType );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	�ж��Ƿ�Ϊ��Ч�Ķ��� 
//---------------------------------------------------//
CORE_API bool IsValidObject( xObject hObject );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	��ȡ������ 
//---------------------------------------------------//
CORE_API xObject GetParentObject( xObject hObject );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	����Ӷ��� 
//---------------------------------------------------//
CORE_API bool AddChild( xObject hParent, xObject hChild );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	ɾ���Ӷ��� 
//---------------------------------------------------//
CORE_API void RemoveChild( xObject hParent, xObject hChild );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	 ��ѯ�Ӷ���
//---------------------------------------------------//
CORE_API bool QueryChild( xObject hParent, xObject hChild );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	���ٶ��� 
//---------------------------------------------------//
CORE_API void DestroyObject( xObject hObject );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	��ȡ�ַ�������
//---------------------------------------------------//
CORE_API _lpctstr GetStrAttrib( xObject hObject, _lpctstr lpszKey );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	�����ַ������� 
//---------------------------------------------------//
CORE_API void SetStrAttrib( xObject hObject, _lpctstr lpszKey, _lpctstr lpszValue );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	��ȡ��������
//---------------------------------------------------//
CORE_API int GetIntegerAttrib( xObject hObject, _lpctstr lpszKey, int nDefault = 0 );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	������������ 
//---------------------------------------------------//
CORE_API void SetIntegerAttrib( xObject hObject, _lpctstr lpszKey, int nValue );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	��ȡ��������
//---------------------------------------------------//
CORE_API float GetFloatAttrib( xObject hObject, _lpctstr lpszKey, float fDefault = 0.0f );

//---------------------------------------------------//
// [12/15/2010 Albert]
// Description:	������������ 
//---------------------------------------------------//
CORE_API void SetFloatAttrib( xObject hObject, _lpctstr lpszKey, float fValue );

//---------------------------------------------------//
// [12/14/2010 Albert]
// Description:	���ô�����Reciver 
//---------------------------------------------------//
CORE_API void SetTriggerReciver( xObject hObject, int nEvent, void (*Reciver)( xObject, xObject, long_ptr, int_ptr ) );

//---------------------------------------------------//
// [12/23/2010 Albert]
// Description:	�����¼� 
//---------------------------------------------------//
CORE_API void TriggerEvent( xType nType, xObject hSource, xObject hTarget, long_ptr lParam, int_ptr wParam );

//---------------------------------------------------//
// [1/7/2011 Albert]
// Description:	���ý�ɫ״̬ 
//---------------------------------------------------//
CORE_API void SetActorStatus( xObject hActor, int eStatus, float fDelayTime = 0.0f, int nMode = 0 );

//---------------------------------------------------//
// [1/13/2011 Albert]
// Description:	��ȡ��ɫ״̬ 
//---------------------------------------------------//
CORE_API int GetActorStatus( xObject hActor );

//////////////////////////////////////////////////////////////////////////
// Helper Functor
//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------//
// [12/1/2010 Albert]
// Description:	��ȡ����ĵ�ǰ���� 
//---------------------------------------------------//
CORE_API const XVector3& GetPosition( xObject hObject );

//---------------------------------------------------//
// [1/7/2011 Albert]
// Description:	��ȡ��ǰ���� 
//---------------------------------------------------//
CORE_API const XVector3& GetDirection( xObject hObject );

//---------------------------------------------------//
// [1/7/2011 Albert]
// Description:	��̬�ƶ�����ָ��λ��
//---------------------------------------------------//
CORE_API bool MoveObjectTo( xObject hObject, XVector3& vPosition, bool bTestCollision = true );

//---------------------------------------------------//
// [11/18/2010 Albert]
// Description:	��ȡһ����Χ�ڵ������ 
//---------------------------------------------------//
CORE_API int RandomRange( int nMin, int nMax );

//---------------------------------------------------//
// [12/1/2010 Albert]
// Description:	 ��ȡһ����Χ�ڵ������ 
//---------------------------------------------------//
CORE_API float RandomRange( float fMin, float fMax );

//---------------------------------------------------//
// [11/19/2010 Albert]
// Description:	��ɫ�ܵ��˺� 
//---------------------------------------------------//
CORE_API void AttackTarget( xObject hAttacker, xObject hInjured, int nDamage );

//---------------------------------------------------//
// [11/29/2010 Albert]
// Description:	ɨ����Χ�Ľ�ɫ 
//---------------------------------------------------//
CORE_API xObject ScanRange( xObject hObject, float fRangeMin, float fRangeMax, xObject (*CombinResult)( xObject hObject[], size_t nCount ), bool (*Fillter)( xObject hObject, long_ptr lParam ), long_ptr lParam );
#endif // _XUTILITY_FUNCTOR_H
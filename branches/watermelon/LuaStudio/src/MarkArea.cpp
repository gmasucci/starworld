/*-----------------------------------------------------------------------------
	Lua Studio

Copyright (c) 1996-2008 Michal Kowalski
-----------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "MarkArea.h"


void CMarkArea::SetEnd(int end)
{
  ASSERT(start >= 0);		// przed 'SetEnd' musi by� wywo�ane 'SetStart'
  ASSERT(end >= start);		// b��dne kra�ce przedzia��w
  if (end < start)		// b��dne kra�ce przedzia��w?
    return;

  Pair pair= {start,end};
  for (UINT i=0; i<n; i++)
  {
    if (arr[i].a > end || arr[i].b < start)
      continue;			// przedzia�y roz��czne
    if (arr[i].a <= start)
      if (arr[i].b >= end)
	return;			// nowa para mie�ci si� w przedziale
      else
      {
	arr[i].b = end;		// przesuni�cie ko�ca przedzia�u
	return;
      }
    else if (arr[i].b <= end)
    {
      arr[i].a = start;		// przesuni�cie pocz�tku przedzia�u
      return;
    }
    else
    {
      arr[i].a = start;		// poszerzenie ca�ego przedzia�u
      arr[i].b = end;
      return;
    }
  }
  arr.SetAtGrow(n,pair);
  n++;
}


bool CMarkArea::GetPartition(UINT no, int &a, int &b)
{
  if (no >= n)
    return FALSE;
  a = arr[no].a;
  b = arr[no].b;
  return true;
}

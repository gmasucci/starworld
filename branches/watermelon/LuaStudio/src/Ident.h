/*-----------------------------------------------------------------------------
	Lua Studio

Copyright (c) 1996-2008 Michal Kowalski
-----------------------------------------------------------------------------*/

#ifndef _ident_h_
#define _ident_h_

struct CIdent
{
  enum IdentInfo	// info o identyfikatorze
  {
    I_INIT,
    I_UNDEF,		// identyfikator niezdefiniowany
    I_ADDRESS,		// identyfikator zawiera adres
    I_VALUE,		// identyfikator zawiera warto�� liczbow�
    I_MACRONAME,	// identyfikator jest nazw� makrodefinicji
    I_MACROADDR		// identyfikator zawiera adres w makrodefinicji
  } info;
  SINT32 val;		// warto�� liczbowa
  UINT8 checked;	// definicja identyfikatora potwierdzona w drugim przej�ciu asemblacji
  UINT8 variable;	// identyfikator zmiennej

  CIdent() : info(I_INIT), checked(FALSE), variable(FALSE)
  { }

  CIdent(IdentInfo info, SINT32 value= 0, bool variable= FALSE) :
    info(info), val(value), checked(FALSE), variable(variable)
  { }
};

#endif

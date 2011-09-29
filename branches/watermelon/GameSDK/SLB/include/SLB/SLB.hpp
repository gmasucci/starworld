/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __SLB__
#define __SLB__

#include "Manager.hpp"
#include "ClassInfo.hpp"
#include "PushGet.hpp"
#include "Type.hpp"
#include "Table.hpp"
#include "Value.hpp"
#include "Class.hpp"
#include "LuaObject.h"
#include "LuaTableIterator.h"
#include "Hybrid.hpp"
#include "StatefulHybrid.hpp"
#include "Script.hpp"
#include "SPP.hpp"
#include "util.hpp"

// just for help....
#define SLB_ON_LOAD( FuncName ) \
	SPP_STATIC_BLOCK( FuncName(); )

#define SLB_abs_index(L, i)     ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : \
		                    lua_gettop(L) + (i) + 1)


#endif

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

#include <SLB/ClassInfo.hpp>
#include <SLB/Manager.hpp>
#include <SLB/Hybrid.hpp>
#include <SLB/Debug.hpp>
#include <SLB/util.hpp>

namespace SLB {

	ClassInfo::ClassInfo(const std::type_info &ti) :
		Namespace(true),
		_typeid(0), 
		_name(""), 
		_instanceFactory(0),
		_isHybrid(false)
	{
		SLB_DEBUG_CALL;
		_typeid = &ti;
		Manager::getInstance().addClass(this);
		_name = _typeid->name();
	}

	ClassInfo::~ClassInfo()
	{
		SLB_DEBUG_CALL;
		delete _instanceFactory; 
	}

	void ClassInfo::setName(const std::string& name)
	{
		SLB_DEBUG_CALL;
		// rename first in the manager...
		Manager::getInstance().rename(this, name);
		_name = name;
	}
	
	void ClassInfo::getMembers( std::list< const char* >& l, bool baseClass )
	{
		Table* t = dynamic_cast< Table* >( get( std::string( "__members" ) ) );
		if( t )
		{
			t->getElements( l );
		}
		if( baseClass )
		{
			for(BaseClassMap::const_iterator b = _baseClasses.begin(); b != _baseClasses.end(); ++b )
			{
				((ClassInfo*)b->second.get())->getMembers( l, baseClass );
			}
		}
	}

	void ClassInfo::setInstanceFactory( InstanceFactory *factory)
	{
		SLB_DEBUG_CALL;
		delete _instanceFactory; // delete old..
		_instanceFactory = factory;
	}

	void ClassInfo::setConstructor( FuncCall *constructor )
	{
		SLB_DEBUG_CALL;
		_constructor = constructor;
	}
	
	void ClassInfo::setClass__index( FuncCall *func )
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "ClassInfo(%p) '%s' set Class __index -> %p", this, _name.c_str(), func);
		_meta__index[0] = func;
	}

	void ClassInfo::setClass__newindex( FuncCall *func )
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "ClassInfo(%p) '%s' set Class __newindex -> %p", this, _name.c_str(), func);
		_meta__newindex[0] = func;
	}

	void ClassInfo::setObject__index( FuncCall *func )
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "ClassInfo(%p) '%s' set Object __index -> %p", this, _name.c_str(), func);
		_meta__index[1] = func;
	}

	void ClassInfo::setObject__newindex( FuncCall *func )
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "ClassInfo(%p) '%s' set Object __newindex -> %p", this, _name.c_str(), func);
		_meta__newindex[1] = func;
	}

	void ClassInfo::pushImplementation(lua_State *L)
	{
		Table::pushImplementation(L);
		lua_getmetatable(L, -1);
		
		lua_pushstring(L, "__objects");
		lua_newtable(L);
		lua_rawset(L, -3);
		
		lua_pushstring(L, "__class_ptr");
		lua_pushlightuserdata(L, (void*)this);
		lua_rawset(L, -3);

		lua_pop(L,1); // remove metatable
	}

	void ClassInfo::pushInstance(lua_State *L, InstanceBase *instance)
	{
		SLB_DEBUG_CALL;
		int top = lua_gettop(L);
		if (instance == 0)
		{
			SLB_DEBUG(7, "Push(%s) Invalid!!", _name.c_str());
			lua_pushnil(L);
			return;
		}
		
		InstanceBase **obj = reinterpret_cast<InstanceBase**>
			(lua_newuserdata(L, sizeof(InstanceBase*))); // top+1
		*obj = instance;

		SLB_DEBUG(7, "Push(%s) InstanceHandler(%p) -> ptr(%p) const_ptr(%p)",
				_name.c_str(), instance, instance->get_ptr(), instance->get_const_ptr());

		// get metatable (class table's metatable)
		push(L);  // (table) top+2
		lua_getmetatable(L,-1);
		lua_replace(L,-2);

		lua_pushvalue(L,-1);
		lua_setmetatable(L, top+1);

		// keep a copy
		if (instance->keepCopyAsCache())
		{
			lua_getfield(L, top+2, "__objects");
			lua_pushlightuserdata(L, const_cast<void*>(instance->get_const_ptr()) );
			lua_pushvalue(L,top+1);
			lua_rawset(L, -3);
		}

		lua_settop(L, top+1);

	}

	void *ClassInfo::get_ptr(lua_State *L, int pos) const
	{
		SLB_DEBUG_CALL;
		pos = L_abs_index(L,pos);
		void *obj = 0;
		InstanceBase *i = getInstance(L, pos);
		if (i)
		{
			ClassInfo *i_ci = i->getClass();
			assert("Invalid ClassInfo" && (i_ci != 0));
			obj = Manager::getInstance().convert( 
					i_ci->getTypeid(), 
					getTypeid(), 
					i->get_ptr()
				);
		}
		SLB_DEBUG(7, "Class(%s) get_ptr at %d -> %p", _name.c_str(), pos, obj);
		return obj;
	}

	const void* ClassInfo::get_const_ptr(lua_State *L, int pos) const
	{
		SLB_DEBUG_CALL;
		pos = L_abs_index(L,pos);
		const void *obj = 0;
		InstanceBase *i = getInstance(L, pos);
		if (i)
		{
			ClassInfo *i_ci = i->getClass();
			assert("Invalid ClassInfo" && (i_ci != 0));
			obj = Manager::getInstance().convert( 
					i_ci->getTypeid(),
					getTypeid(),
					i->get_const_ptr() );
		}
		SLB_DEBUG(7, "Class(%s) get_const_ptr -> %p", _name.c_str(), obj);
		return obj;
	}

	InstanceBase* ClassInfo::getInstance(lua_State *L, int pos) const
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(10, "L=%p; Pos = %i (abs = %i)",L, pos, L_abs_index(L,pos) );
		pos = L_abs_index(L,pos);
		InstanceBase *instance = 0;
		int top = lua_gettop(L);
		if (lua_getmetatable(L, pos))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				void *obj = lua_touserdata(L, pos);
				if (obj == 0) 
				{
					luaL_error(L,
						"Expected object of type %s at #%d", 
						_name.c_str(), pos);
				}
				instance = *reinterpret_cast<InstanceBase**>(obj);
			}
		}
		lua_settop(L, top);
		SLB_DEBUG(10, "Instance(%p) -> %p (%s)",
			instance,
			instance? instance->get_const_ptr() : 0,
			instance? (instance->get_const_ptr() ? "const" : "non_const") : "nil");
		return instance;
	}

	void ClassInfo::push_ref(lua_State *L, void *ref )
	{
		SLB_DEBUG_CALL;
		if (_instanceFactory)
		{
			if (ref)
			{
				pushInstance(L, _instanceFactory->create_ref(ref) );
				SLB_DEBUG(7, "Class(%s) push_ref -> %p", _name.c_str(), ref);
			}
			else
			{
				luaL_error(L, "Can not push a NULL reference of class %s", _name.c_str());
			}
		}
		else
		{
			luaL_error(L, "Unknown class %s (push_reference)", _name.c_str());
		}
	}

	void ClassInfo::push_ptr(lua_State *L, void *ptr, bool fromConstructor)
	{
		SLB_DEBUG_CALL;
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_ptr(ptr, fromConstructor) );
			SLB_DEBUG(7, "Class(%s) push_ptr (from_Constructor %d) -> %p", _name.c_str(), fromConstructor, ptr);

			// if is Hybrid and fromConstructor
			if (_isHybrid && fromConstructor)
			{
				int top = lua_gettop(L);
				//suppose that this state will hold the Object..
				HybridBase *hb = SLB::get<HybridBase*>(L,top);
				if (!hb) assert("Invalid push of hybrid object" && false);
				if (!hb->isAttached()) hb->attach(L);
				// check... just in case
				assert("Invalid lua stack..." && (top == lua_gettop(L)));
			}

		}
		else
		{
			luaL_error(L, "Can not push a ptr of class %s", _name.c_str());
		}
	}

	void ClassInfo::push_const_ptr(lua_State *L, const void *const_ptr)
	{
		SLB_DEBUG_CALL;
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_const_ptr(const_ptr) );
			SLB_DEBUG(7, "Class(%s) push const_ptr -> %p", _name.c_str(), const_ptr);
		}
		else
		{
			luaL_error(L, "Can not push a const_ptr of class %s", _name.c_str());
		}
	}

	void ClassInfo::push_copy(lua_State *L, const void *ptr)
	{
		SLB_DEBUG_CALL;
		if (_instanceFactory)
		{
			if (ptr)
			{
				pushInstance(L, _instanceFactory->create_copy(ptr) );
				SLB_DEBUG(7, "Class(%s) push copy -> %p", _name.c_str(), ptr);
			}
			else
			{
				luaL_error(L, "Can not push copy from NULL of class %s", _name.c_str());
			}
		}
		else
		{
			luaL_error(L, "Can not push a copy of class %s", _name.c_str());
		}
	}
	
	int ClassInfo::__index(lua_State *L)
	{
		SLB_DEBUG_CALL;
		int result = Table::__index(L); // default implementation
		if ( result < 1 )
		{
			// type == 0 -> class __index
			// type == 1 -> object __index
			int type = lua_istable(L,1)? 0 : 1; 
			SLB_DEBUG(4, "Called ClassInfo(%p) '%s' __index %s", this, _name.c_str(), type? "OBJECT" : "CLASS");

			// if looking for a class method, using an string...
			if (lua_isstring(L,2))
			{
				const char *key = lua_tostring(L,2);
				Object *obj = 0;
				if( lua_gettop(L) >= 2 )
				{
					std::string key( "__members::" );
					key = key + lua_tostring(L,2) + "::__get";
					Object* member = get( key );
					if( member )
					{
						member->push(L);
						lua_insert(L,1);
						lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
						result = lua_gettop(L);
					}
				}

				for(BaseClassMap::iterator i = _baseClasses.begin(); obj == 0L && i != _baseClasses.end(); ++i)
				{
					obj = ((Table*)(i->second.get()))->get(key);
					if (obj)
					{
						obj->push(L);
						result = 1;
						break;
					}
					else
					{
						std::string get_key( "__members::" );
						get_key = get_key + key + "::__get";
						Object* member = ((Table*)(i->second.get()))->get( get_key );
						if( member )
						{
							member->push(L);
							lua_insert(L,1);
							lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
							result = lua_gettop(L);
							break;
						}
					}
				}
			}

			if (result < 1) // still not found...
			{
				if (_meta__index[type].valid())
				{
					// 1 - func to call
					_meta__index[type]->push(L);
					lua_insert(L,1);	
					// 2 - object/table
					// 3 - key
					SLB_DEBUG_STACK(8,L,  "Class(%s) __index {%s} metamethod -> %p",
						_name.c_str(), type? "OBJECT" : "CLASS", (void*)_meta__index[type].get() );

					assert("Error in number of stack elements" && lua_gettop(L) == 3);
					lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
					result = lua_gettop(L);
				}
				else
				{
					SLB_DEBUG(4, "ClassInfo(%p) '%s' doesn't have __index[%s] implementation.",
						this, _name.c_str(), type? "OBJECT" : "CLASS");
				}
			}
		}

		return result;
	}

	int ClassInfo::__newindex(lua_State *L)
	{
		SLB_DEBUG_CALL;
		// 0 = class __index
		// 1 = object __index
		int type = lua_istable(L,1)? 0 : 1; 
		SLB_DEBUG(4, "Called ClassInfo(%p) '%s' __newindex %s", this, _name.c_str(), type? "OBJECT" : "CLASS");
		if (_meta__newindex[type].valid())
		{
			// 1 - func to call
			_meta__newindex[type]->push(L);
			lua_insert(L,1);
			// 2 - object
			// 3 - key/table
			// 4 - value
			SLB_DEBUG_STACK(8,L,  "Class(%s) __index {%s} metamethod -> %p",
				_name.c_str(), type?  "OBJECT" :"CLASS", (void*)_meta__newindex[type].get() );

			assert("Error in number of stack elements" && lua_gettop(L) == 4);
			lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
			return lua_gettop(L);
		}
		else
		{
			SLB_DEBUG(4, "ClassInfo(%p) '%s' doesn't have __newindex[%s] implementation.",
				this, _name.c_str(), type? "OBJECT" : "CLASS");
		}

		std::string key( "__members::" );
		if( lua_gettop(L) != 3 ) return 0;
		key = key + lua_tostring(L,2) + "::__set";
		Object* member = get( key );
		if( member )
		{
			member->push(L);		// f
			lua_insert(L,1);	
			lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
			return lua_gettop(L);
		}
		else
		{
			for(BaseClassMap::iterator i = _baseClasses.begin(); i != _baseClasses.end(); ++i)
			{
				Object* member = ((Table*)(i->second.get()))->get( key );
				if( member )
				{
					member->push(L);
					lua_insert(L,1);
					lua_call(L,lua_gettop(L)-1, LUA_MULTRET);
					return lua_gettop(L);
				}
			}
		}

		return Table::__newindex(L);
	}
	
	int ClassInfo::__garbageCollector(lua_State *L)
	{
		InstanceBase* instance = 
			*reinterpret_cast<InstanceBase**>(lua_touserdata(L, 1));
		delete instance; 
		return 0;
	}

	int ClassInfo::__call(lua_State *L)
	{
		SLB_DEBUG_CALL;
		if ( _constructor.valid() )
		{
			_constructor->push(L);
			lua_replace(L, 1); // remove ClassInfo table
			lua_call(L, lua_gettop(L)-1 , LUA_MULTRET);
			return lua_gettop(L);
		}
		else
		{
			luaL_error(L,
				"ClassInfo '%s' is abstract ( or doesn't have a constructor ) ", _name.c_str());
		}
		return 0;
	}

	int ClassInfo::__tostring(lua_State *L)
	{
		SLB_DEBUG_CALL;
		int top = lua_gettop(L);
		lua_pushfstring(L, "Class(%s) [%s]", _name.c_str(), getInfo().c_str());
		;
		for(BaseClassMap::iterator i = _baseClasses.begin(); i != _baseClasses.end(); ++i)
		{
				lua_pushfstring(L, "\n\tinherits from %s (%p)",i->second->getName().c_str(), (Object*) i->second.get());
		}
		for(Elements::iterator i = _elements.begin(); i != _elements.end(); ++i)
		{
			Object *obj = i->second.get();
			FuncCall *fc = dynamic_cast<FuncCall*>(obj);
			if (fc)
			{
				lua_pushfstring(L, "\n\tfunction (%s) [%s]",i->first.c_str(), obj->getInfo().c_str() );
				for (size_t i = 0; i < fc->getNumArguments(); ++i)
				{
					lua_pushfstring(L, "\n\t\t[%d] (%s) [%s]",i,
						fc->getArgType(i)->name(),
						fc->getArgComment(i).c_str()
						);
				}
				//TODO, print return type
			}
			else
			{
				lua_pushfstring(L, "\n\t%s -> %p [%s] [%s]",i->first.c_str(), obj, typeid(*obj).name(), obj->getInfo().c_str() );
			}
		}
		lua_concat(L, lua_gettop(L) - top);
		return 1;
	}

	bool ClassInfo::isSubClassOf( const ClassInfo *base )
	{
		SLB_DEBUG_CALL;
		if (base == this) return true;
		BaseClassMap::iterator i = _baseClasses.find( base->getTypeid() );
		return (i != _baseClasses.end());
	}

}

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

#ifndef __SLB_CLASS_INFO__
#define __SLB_CLASS_INFO__

#include "Export.hpp"
#include "Object.hpp"
#include "Instance.hpp"
#include "Table.hpp"
#include "ref_ptr.hpp"
#include "FuncCall.hpp"
//#include "ClassHelpers.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>
#include <list>
struct lua_State;

namespace SLB {

	class SLB_EXPORT Namespace : public Table
	{
	public:
		Namespace( bool cacheable = true ) : Table("::", cacheable) {}
	protected:
		virtual ~Namespace() {}
	};

	class SLB_EXPORT ClassInfo : public Namespace
	{
	public:
		typedef std::map<const std::type_info*, ref_ptr<ClassInfo> > BaseClassMap;
		typedef std::list< std::pair< const std::type_info*, const std::type_info* > >	ConverList;

		const std::type_info *getTypeid() const { return _typeid; }
		const std::string &getName() const      { return _name; }
		void getMembers( std::list< const char* >& l, bool baseClass = true );
		void setName(const std::string&);

		void push_ref(lua_State *L, void *ref);
		void push_ptr(lua_State *L, void *ptr, bool fromConstructor = false);
		void push_const_ptr(lua_State *L, const void *const_ptr);
		void push_copy(lua_State *L, const void *ptr);
		void* get_ptr(lua_State*, int pos) const;
		const void* get_const_ptr(lua_State*, int pos) const;

		template< class C, typename V >
		void member( const char* name, V C::* offset )
		{
			std::string key( "__members::" );
			set( key + name + "::__get", getMember< C, V >::create( offset ) );
			set( key + name + "::__set", setMember< C, V >::create( offset ) );
		}

		static int _readonly( lua_State *L )
		{
			luaL_error( L, "param is readonly value!" );
			return 0;
		}

		template< class C, typename V >
		void member_readonly( const char* name, V C::* offset )
		{
			std::string key( "__members::" );
			set( key + name + "::__get", getMember< C, V >::create( offset ) );
			set( key + name + "::__set", FuncCall::create( _readonly ) );
		}

		template< class setMethod,	class getMethod >
		void member( const char* name, setMethod s, getMethod g )
		{
			std::string key( "__members::" );
			set( key + name + "::__get", FuncCall::create( g ) );
			set( key + name + "::__set", FuncCall::create( s ) );
		}

		template< class setMethod >
		void member_set( const char* name, setMethod s )
		{
			std::string key( "__members::" );
			set( key + name + "::__get", FuncCall::create( s ) );
		}

		template< class getMethod >
		void member_get( const char* name, getMethod g )
		{
			std::string key( "__members::" );
			set( key + name + "::__set", FuncCall::create( g ) );
		}

		// Uses dynamic_cast to convert from Base to Derived
		template<class This, class Base>
		void inheritsFrom();

		// This version uses static cast instead of dynamic_cast
		template<class This, class Base>
		void staticInheritsFrom();


		void setConstructor( FuncCall *constructor );
		void setInstanceFactory( InstanceFactory *);

		/** __index method will receive:
		 *  - object
		 *  - key */
		void setObject__index( FuncCall* );

		/** __index method will receive:
		 *  - object
		 *  - key
		 *  - value */
		void setObject__newindex( FuncCall* );

		/** Here you can use setCache/getCache methods to
		 * speed up indexing.
		 *
		 * __index method will receive:
		 *  - [table] -> ClassInfo
		 *  - key */
		void setClass__index( FuncCall* );

		/** Here you can use setCache/getCache methods to
		 * speed up indexing.
		 * __index method will receive:
		 *  - [table] -> ClassInfo
		 *  - key
		 *  - value */
		void setClass__newindex( FuncCall* );

		//This is used by some default initializations...
		bool initialized() const { return _instanceFactory != 0; }

		bool isSubClassOf( const ClassInfo*, ConverList* l = NULL, bool bBaseToDerived = false );
		bool hasConstructor() const { return _constructor.valid(); }

		//--Private methods -(not meant to be used)-------------------
		void setHybrid() { _isHybrid = true; }
		FuncCall* getConstructor() { return _constructor.get(); }

	protected:
		ClassInfo(const std::type_info&);
		virtual ~ClassInfo();
		void pushImplementation(lua_State *);
		virtual int __index(lua_State*);
		virtual int __newindex(lua_State*);
		virtual int __call(lua_State*);
		virtual int __garbageCollector(lua_State*);
		virtual int __tostring(lua_State*);

		const std::type_info *_typeid;
		std::string       _name;
		InstanceFactory  *_instanceFactory;
		BaseClassMap      _baseClasses;
		ref_ptr<FuncCall> _constructor;
		ref_ptr<FuncCall> _meta__index[2]; // 0 = class, 1 = object
		ref_ptr<FuncCall> _meta__newindex[2]; // 0 = class, 1 = object
		bool _isHybrid;

	private:
		void pushInstance(lua_State *L, InstanceBase *instance);
		InstanceBase* getInstance(lua_State *L, int pos) const;

		friend class Manager;
	};


	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	template<class D, class B>
	inline void ClassInfo::inheritsFrom()
	{
		Manager::getInstance().addConversor<D,B>();
		_baseClasses[ &typeid(B) ] = Manager::getInstance().getOrCreateClass(typeid(B));
	}

	template<class D, class B>
	inline void ClassInfo::staticInheritsFrom()
	{
		Manager::getInstance().addStaticConversor<D,B>();
		_baseClasses[ &typeid(B) ] = Manager::getInstance().getOrCreateClass(typeid(B));
	}

}


#endif

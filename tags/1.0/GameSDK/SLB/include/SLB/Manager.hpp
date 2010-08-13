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

#ifndef __SLB_MANAGER__
#define __SLB_MANAGER__

#include "Object.hpp"
#include "ref_ptr.hpp"
#include "Export.hpp"
#include "Debug.hpp"
#include <typeinfo>
#include <map>
#include <cstdlib>

namespace SLB {

	class ClassInfo;
	class Namespace;

	// copy values and objects from one lua_State to another
	bool copy(lua_State *from, int pos, lua_State *to);

	class SLB_EXPORT Manager
	{
	public:
		typedef std::map< const std::type_info*, ref_ptr<ClassInfo> > ClassMap;
		typedef std::map< const std::string, const std::type_info *> NameMap;
		typedef void* (*Conversor)(void *);
		typedef std::map< std::pair<const std::type_info*, const std::type_info*>, Conversor > ConversionsMap;

		static Manager &getInstance();
		static Manager *getInstancePtr();

		const ClassInfo *getClass(const std::type_info&) const;
		const ClassInfo *getClass(const std::string&) const;
		/// Returns the classInfo of an object, or null if it is a basic lua-type
		ClassInfo *getClass(lua_State *L, int pos) const ;
		ClassInfo *getClass(const std::type_info&);
		ClassInfo *getClass(const std::string&);
		ClassInfo *getOrCreateClass(const std::type_info &);

		/** Copy from one lua_State to another:
		  - for basic types it will make a basic copy
		  - returns true if copy was made, otherwise returns false. 
		  - doesn't touch the original element

		  ** WARNING **
		  copy of tables is not yet implemented...
		*/
		bool copy(lua_State *from, int pos, lua_State *to);

		// set a global value ( will be registered authomatically on every lua_State )
		void set(const std::string &, Object *obj);

		// This will add a SLB table to the current global state
		void registerSLB(lua_State *L);

		// convert from C1 -> C2
		void* convert( const std::type_info *C1, const std::type_info *C2, void *obj);
		const void* convert( const std::type_info *C1, const std::type_info *C2, const void *obj);

		Namespace* getGlobals() { return _global.get(); }

		/** Resets SLB clearing every class data. It is used only to check memory leaks, it should not be
		 * used in normal runtime...
		 */
		static void reset();
	protected:

		void rename(ClassInfo *c, const std::string &new_name);
		void addClass( ClassInfo *c );
		template<class Derived, class Base>
		void addConversor();
		template<class Derived, class Base>
		void addStaticConversor();

		/** Returns the classMap with all defined classes */
		ClassMap& getClasses() { return _classes; }

		friend int SLB_allTypes(lua_State *);

	private:
		Manager();
		~Manager();
		Manager(const Manager&);
		Manager& operator=(const Manager&);

		ClassMap _classes;
		NameMap  _names;
		ref_ptr<Namespace> _global;
		ConversionsMap _conversions;

		static Manager *_singleton;
		friend class ClassInfo;
	};
	
	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	template<class D, class B>
	struct ClassConversor
	{
		static void* convertToBase(void *raw_d)
		{
			D* derived = reinterpret_cast<D*>(raw_d);
			B* base = derived;
			return (void*) base;
		}
		
		static void* convertToDerived(void *raw_b)
		{
			B* base = reinterpret_cast<B*>(raw_b);
			D* derived = dynamic_cast<D*>(base);
			return (void*) derived;
		}

		static void* staticConvertToDerived(void *raw_b)
		{
			B* base = reinterpret_cast<B*>(raw_b);
			D* derived = static_cast<D*>(base);
			return (void*) derived;
		}
		
	};
	
	inline Manager &Manager::getInstance()
	{
		return *getInstancePtr();
	}

	template<class D, class B>
	inline void Manager::addConversor()
	{
		_conversions[ ConversionsMap::key_type(&typeid(D), &typeid(B)) ] = &ClassConversor<D,B>::convertToBase;
		_conversions[ ConversionsMap::key_type(&typeid(B), &typeid(D)) ] = &ClassConversor<D,B>::convertToDerived;
	}

	template<class D, class B>
	inline void Manager::addStaticConversor()
	{
		_conversions[ ConversionsMap::key_type(&typeid(D), &typeid(B)) ] = &ClassConversor<D,B>::convertToBase;
		_conversions[ ConversionsMap::key_type(&typeid(B), &typeid(D)) ] = &ClassConversor<D,B>::staticConvertToDerived;
	}

	inline void* convert( const std::type_info *C1, const std::type_info *C2, void *obj);

	inline const void* Manager::convert( const std::type_info *C1, const std::type_info *C2, const void *obj)
	{
		return const_cast<void*>(convert(C1,C2, const_cast<void*>(obj)));
	}
	
	inline Manager *Manager::getInstancePtr()
	{
		if (_singleton == 0)
		{
			_singleton = new Manager();
			atexit(Manager::reset);
		}
		return _singleton;
	}

	inline bool copy(lua_State *from, int pos, lua_State *to)
	{
		return Manager::getInstance().copy(from,pos,to);
	}

}

#endif

#include <SLB/Iterator.hpp>
#include <SLB/Debug.hpp>
namespace SLB {

	Iterator::Iterator(IteratorBase *b) : _iterator(b)
	{
		SLB_DEBUG_CALL;
	}

	void Iterator::pushImplementation(lua_State *L)
	{
		SLB_DEBUG_CALL;
		lua_pushlightuserdata(L, (void*) _iterator );
		lua_pushcclosure( L, Iterator::iterator_call, 1 );
	}

	Iterator::~Iterator()
	{
		SLB_DEBUG_CALL;
		delete _iterator;
	}

	int Iterator::iterator_call(lua_State *L)
	{
		SLB_DEBUG_CALL;
		IteratorBase *ib = reinterpret_cast<IteratorBase*>( lua_touserdata(L, lua_upvalueindex(1) ) );
		return ib->push(L);
	}

}

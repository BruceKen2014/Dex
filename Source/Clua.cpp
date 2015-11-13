

#include "CLua.h"


void   _lua_getfield(lua_State* L, char* key, void* ret, int flag)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	switch (flag)
	{
	case LUA_INT:
		{
			(*(int*)ret) = (int)lua_tointeger(L, -1);
			break;
		}
	case LUA_POINT:
		{
			ret = (void*)lua_topointer(L, -1);
			break;
		}
	case LUA_DOUBLE:
		{
			(*(double*)ret) = (double)lua_tonumber(L, -1);
			break;
		}
	case LUA_STRING:
		{
			strcpy((char*)ret, lua_tostring(L, -1));
			break;
		}
	default:
		break;
	}
	lua_pop(L, 1);
};
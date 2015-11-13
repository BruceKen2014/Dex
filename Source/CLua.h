

#pragma  once
#include "../DexBase/typedefine.h"


#define  LUA_INT     0x0000
#define  LUA_BOOL	 0x0001
#define  LUA_STRING  0x0002
#define  LUA_DOUBLE  0x0003
#define  LUA_POINT   0x0004

void   _lua_getfield(lua_State* L, char* key, void* ret, int flag);
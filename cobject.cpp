#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "cobject.h"

//初始化Lua的调用表
DECL_LUA_METHOD_TABLE_BEGIN( cobject )
#include "cobject.def"
DECL_LUA_METHOD_TABLE_END
// binder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "cobject.h"

#pragma comment(lib,"lua.lib")

/*
* lua 扩展函数用于传输返回值
LUA_API int lua_push( lua_State * L , int type , __int64 result )
{
    int res = 1;
    switch ( novariant( type ) )
    {
        //参数不可能 NONE
    case LUA_TNONE:
        res = 0;
        break;
    case LUA_TNIL:
        lua_pushnil( L );
        break;
    case LUA_TBOOLEAN:
        lua_pushboolean( L , (int)result );
        break;
    case LUA_TNUMBER:
        if ( type & 0x0F0 )
        {
            lua_pushinteger( L , *(__int64 *)&result );
        } else
        {
            lua_pushnumber( L , *(double *)&result );
        }break;
    case LUA_TSTRING:
        lua_pushstring( L , *(const char **)&result );
        break;
    case LUA_TTABLE:
        lua_pushlightuserdata( L , *(void **)&result );
        break;
    case LUA_TFUNCTION:
        lua_pushcfunction( L , *(lua_CFunction *)&result );
        break;
    case LUA_TUSERDATA:
        lua_pushlightuserdata( L , *(void **)&result );
        break;
    case LUA_TLIGHTUSERDATA:
        lua_pushlightuserdata( L , *(void **)&result );
        break;
    default:
        res = 0;
        break;
    }
    return res;
}

// lua 扩展函数 用于自识别类型
LUA_API unsigned __int64 lua_trait( lua_State * L , int index )
{
    unsigned __int64 value = 0;
    TValue * v = index2value( L , index );
    if ( NULL == v ) return 0;

    switch ( ttypetag( v ) )
    {
    case LUA_TNONE:
    case LUA_TNIL:
        break;
    case LUA_VLCL:
        *( (void **)( &value ) ) = clLvalue( v );
        break;
    case LUA_VCCL:
        *( (void **)( &value ) ) = clCvalue( v )->f;
        break;
    case LUA_VLCF:
        *( (void **)( &value ) ) = cast_voidp( cast_sizet( fvalue( v ) ) ); //cast( void * , cast( size_t , fvalue( v ) ) );
        break;
    default:
        switch ( ttype( v ) )
        {
        case LUA_TBOOLEAN:
            {
                *( (int *)( &value ) ) = ( !l_isfalse( v ) ) ? 1 : 0;
            }break;
        case LUA_TSTRING:
            {
                *( (char **)( &value ) ) = svalue( v );
            }break;
        case LUA_TNUMBER:
            if ( v->tt_ & 0x00F0 )
            {
                lua_Number n = 0;
                tonumber( v , &n );
                *( (double *)( &value ) ) = n;
            } else
            {
                lua_Integer n = 0;
                tointeger( v , &n );
                *( (__int64 *)( &value ) ) = n;
            }
            break;
        case LUA_TFUNCTION:
            if ( ttislcf( v ) ) *( (void **)( &value ) ) = (void *)fvalue( v );
            else if ( ttisCclosure( v ) ) *( (void **)( &value ) ) = (void *)clCvalue( v )->f;
            break;
        case LUA_TTABLE:
            *( (void **)( &value ) ) = hvalue( v );
            break;
        case LUA_TTHREAD:
            *( (void **)( &value ) ) = thvalue( v );
            break;
        case LUA_TUSERDATA:
            *( (void **)( &value ) ) = getudatamem( uvalue( v ) );
            break;
        case LUA_TLIGHTUSERDATA:
            *( (void **)( &value ) ) = pvalue( v );
            break;
        default:
            if ( iscollectable( v ) )
                *( (void **)( &value ) ) = gcvalue( v );
            break;
        }
    }

    return value;
}
*/

int main()
{
    std::cout << "Hello World!\n";

    lua_State * L = luaL_newstate( );
    luaL_openlibs( L );

    cdog dog,dog2;

    dog.SetAttribute( "Jack" , 5 );
    
    REG_LUA_METHOD_TABLE( cobject , L );
    

    LUA_SET_OBJECT(L, "dog" , cobject , (cobject *)&dog );

    LUA_SET_OBJECT(L, "dog2" , cobject , (cobject *)&dog2 );

    luaL_dostring( L , "print(\"dog name:\" .. dog:GetName( ) .. \"\\ndog age:\" .. dog:GetAttribute( 1 ) ..\"\\n\" ) dog2:SetAttribute(\"Angle\",3) print(\"dog2 name:\" .. dog2:GetName( ) .. \"\\ndog2 age:\" .. dog2:GetAttribute( 1 ) ..\"\\n\" )" );

    system("pause" );
    lua_close( L );
    return true;
}

/* result
Hello World!
dog name:Jack
dog age:5

dog2 name:Angle
dog2 age:3

请按任意键继续. . .
*/

#pragma once
#include <lua/lua.hpp>

template <typename T>
T Trait( lua_State * L , int index )
{
    unsigned __int64 value = lua_trait( L , index );
    return ( T ) * ( (T *)&value );
};

template <typename T>
int Feedback( lua_State * L , int type , T value )
{
    return lua_push( L , type , *( (__int64 *)&value ));
}

//这里用于区分 double 和 __int64
#define LUA_TFLOAT          (LUA_TNUMBER)
#define LUA_TINT64          (LUA_TNUMBER|0x10)


#define __ARG_INDEX__(t,i) 
#define DEF_REFLECTION_FUN(type,ltype,fun,...)  { #fun , Lua_##fun },

#define __CPP_ARG_INDEX__(t,i) t
#define DEF_CPP_REFLECTION_FUN(type,ltype,fun,...)     virtual type fun ( __VA_ARGS__ ) = 0;

#define __LUA_ARG_INDEX__(t,i) (t)Trait<t>(L,i)
#define DEF_LUA_REFLECTION_FUN(thistype,type,ltype,fun,...)  \
    static int Lua_##fun(lua_State* L)\
    {\
        thistype ** ppThis = (thistype **)lua_topointer(L,1);\
        if ( ppThis != nullptr && *ppThis!=nullptr)\
        {\
            if(ltype>=0)\
            {\
                return Feedback<type>(L,ltype,(*ppThis)->fun( __VA_ARGS__ ));\
            }else\
            {\
                (*ppThis)->fun( __VA_ARGS__ ); \
            }\
        }\
        return 0;\
    }\

//在接口类中定义
#define DECL_LUA_METHOD_TABLE public:\
static luaL_Reg __Lua_Members[];

//在接口类外初始化
#define DECL_LUA_METHOD_TABLE_BEGIN(cls)                luaL_Reg cls##::##__Lua_Members[]={
// #include <xxx.def>
#define DECL_LUA_METHOD_TABLE_END                       {nullptr,nullptr}};

//在lua初始化中调用
#define REG_LUA_METHOD_TABLE(cls,lua)                   {\
                                                            luaL_newmetatable( lua , #cls );\
                                                            lua_pushvalue( lua , -1 );\
                                                            lua_setfield( lua , -2 , "__index" );\
                                                            luaL_setfuncs( lua , cls##::##__Lua_Members , 0 );\
                                                            lua_pop( lua , 1 );\
                                                        }

#define LUA_SET_OBJECT(lua,name,type,object)                {\
                                                            *static_cast<type **>(lua_newuserdata( lua , sizeof(type * ) )) = static_cast<type *>( object );\
                                                            luaL_getmetatable(lua,#type);\
                                                            lua_setmetatable( lua , -2 );\
                                                            lua_setglobal( lua , name );\
                                                        }



// 通过 cobject.def 定义接口
class cobject
{
public:
//定义基类虚函数
#pragma push_macro("DEF_REFLECTION_FUN")
#pragma push_macro("__ARG_INDEX__")
#undef DEF_REFLECTION_FUN
#undef __ARG_INDEX__
#define __ARG_INDEX__(t,i) __CPP_ARG_INDEX__(t,i)
#define DEF_REFLECTION_FUN(type,ltype,fun,...)   DEF_CPP_REFLECTION_FUN(type,ltype,fun,__VA_ARGS__)
#include "cobject.def"

// lua 接口实现部分 Lua_xxx 最终会调用到子类的实现 xxx
#undef DEF_REFLECTION_FUN
#undef __ARG_INDEX__
#define __ARG_INDEX__(t,i) __LUA_ARG_INDEX__(t,i)
#define DEF_REFLECTION_FUN(type,ltype,fun,...)    DEF_LUA_REFLECTION_FUN(cobject,type,ltype,fun,__VA_ARGS__)
#include "cobject.def"
#pragma pop_macro("__ARG_INDEX__")
#pragma pop_macro("DEF_REFLECTION_FUN")

 // 定义lua的函数调用表
    DECL_LUA_METHOD_TABLE
};

//继承接口后可以只需要关注实现部分
class cdog :public cobject
{
protected:
    std::string m_name;
    std::string m_attr;
    int m_age;
public:
    
    virtual bool SetName( const char * name )
    {
        m_name = name;
        return true;
    }

    virtual const char * GetName( )
    {
        return m_name.c_str( );
    }

    virtual int GetAge( )
    {
        return m_age;
    }

    virtual bool SetAttribute( const char * name , int age )
    {
        m_age = age;
        m_name = name;
        return true;
    }
    
    virtual const char * GetAttribute( int attr )
    {
        if ( attr == 0 ) return m_name.c_str( );

        m_attr.resize( 100 );
        _snprintf_s( (char*)m_attr.data() , 100 ,100 , "%d" , m_age );
        return m_attr.c_str( );
    }

};


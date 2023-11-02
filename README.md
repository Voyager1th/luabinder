# luabinder
lua binder,一个简单的 lua c++ 绑定器 就几个宏
只需要关注C++代码的实现,LUA代码可以自动实现
相对 LuaBridge 的太庞大太复杂,这简直不要太简单哦。。。
唯一的缺点就是函数不能时void返回类型,这会导致编译失败，目前没有找到好的解决办法，但是这无关紧要了。
```c++
int main()
{
    std::cout << "Hello World!\n";

    lua_State * L = luaL_newstate( );
    luaL_openlibs( L );

    cdog dog,dog2;
    dog.SetAttribute( "Jack" , 5 );

    //#注册c++方法给lua
    REG_LUA_METHOD_TABLE( cobject , L );

    //#给lua添加一个对象dog
    LUA_SET_OBJECT( "dog" , cobject , (cobject *)&dog );
    
    //#给lua添加一个对象dog2
    LUA_SET_OBJECT( "dog2" , cobject , (cobject *)&dog2 );

    //#执行lua 代码
    luaL_dostring( L , "print(\"dog name:\" .. dog:GetName( ) .. \"\\ndog age:\" .. dog:GetAttribute( 1 ) ..\"\\n\" ) dog2:SetAttribute(\"Angle\",3) print(\"dog2 name:\" .. dog2:GetName( ) .. \"\\ndog2 age:\" .. dog2:GetAttribute( 1 ) ..\"\\n\" )" );

    system("pause" );
    lua_close( L );
    return true;
}
```

在定义基类的时候 c++ 纯虚函数都在 object.def 中定义
以下面方法为例:
```c++
DEF_REFLECTION_FUN( bool, LUA_TBOOLEAN, SetAttribute, __ARG_INDEX__(const char * ,2), __ARG_INDEX__(int ,3) )
```
方法名字 SetAttribute 返回值是 bool ,LUA_TBOOLEAN 是 lua 相对应的数据类型 这样方便我们 传输返回值时自动处理
第一个参数是 __ARG_INDEX__(const char * ,2) 索引从2 开始 就是从lua栈中获取传入的参数 第二个参数是3 以此类推

DEF_REFLECTION_FUN 这个宏会有 3 种形态 不同位置展开的结果不一样

在定义基类成员时是 
```c++
virtual bool SetAttrabute(const char*,int)=0;
```
在定义lua函数时是
```c++
static int Lua_SetAttrabute(LuaState* L)
{
    //因为宏会被转成 DEF_LUA_REFLECTION_FUN(classname,type,ltype,fun,Trait<const char*>(L,2),Trait<int>(L,3))
    classname** ppThis = (classname**)lua_topointer(L,1);
    if(ppThis != nullptr && *ppThis != nullptr)
    {
        if(LUA_TBOOLEAN>=0)
        {
            // 如果函数定义为 void SetAttrabute(const char*,int); //void 返回值不能作为参数 所以导致编译错误，但无关紧要,你可以返回bool值。
            // 可恨 多行宏内部不能使用 条件编译 如果可以有 __if __else __end 该多方便的呢。。。
            return Feedback<bool>(  L,
                                    LUA_TBOOLEAN,
                                    (*ppThis)->SetAttrabute ( Trait<const char*>(L,2), Trait<int>(L,3) ) //Trait 会通过读取lua栈中的数据无关类型
                                  );
        }else
        {
            (*ppThis)->SetAttrabute(Trait<const char*>(L,2),Trait<int>(L,3));
        }
    }
    return 0;
}
```
在开发中需要与Lua交互的类 只需要实现一个基类 复制 class cobject 中的宏,声明代码写在 .def 文件中 然后 #include<xxx.def> 就完事了，只需要在子类实现c++代码 调用 REG_LUA_METHOD_TABLE 注册一下类就完事了。

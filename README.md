# luabinder
lua binder,一个简单的 lua c++ 绑定器 就几个宏
只需要关注C++代码的实现,LUA代码可以自动实现
相对 LuaBridge 的太庞大太复杂,这简直不要太简单哦。。。
```c++
int main()
{
    std::cout << "Hello World!\n";

    lua_State * L = luaL_newstate( );
    luaL_openlibs( L );

    cdog dog,dog2;
    dog.SetAttribute( "Jack" , 5 );

    #注册c++方法给lua
    REG_LUA_METHOD_TABLE( cobject , L );

    #给lua添加一个对象dog
    LUA_SET_OBJECT( "dog" , cobject , (cobject *)&dog );
    
    #给lua添加一个对象dog2
    LUA_SET_OBJECT( "dog2" , cobject , (cobject *)&dog2 );

    #执行lua 代码
    luaL_dostring( L , "print(\"dog name:\" .. dog:GetName( ) .. \"\\ndog age:\" .. dog:GetAttribute( 1 ) ..\"\\n\" ) dog2:SetAttribute(\"Angle\",3) print(\"dog2 name:\" .. dog2:GetName( ) .. \"\\ndog2 age:\" .. dog2:GetAttribute( 1 ) ..\"\\n\" )" );

    system("pause" );
    lua_close( L );
    return true;
}
```


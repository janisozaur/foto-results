#ifndef SCRIPT_H
#define SCRIPT_H


extern "C" {
    #include <Scripts/lua/lua.h>
}

#include <string>
#include <exception>
#include <Scripts/luabind/luabind.hpp>
#include <Scripts/luabind/error.hpp>
#include <QString>

int PrintLastError(lua_State* L);

#define CATCH_EXCEPTIONS(call) try { call; return true;} catch(const luabind::error& e) {return false;}

struct Lua_function_checking_errors {

    lua_State* state;

    template<typename FUNC>
    bool operator()(FUNC const&  func_name)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name))
    }
    template<typename FUNC,typename P1>
    bool operator()(FUNC const&  func_name,P1 p1)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1))
    }
    template<typename FUNC,typename P1,typename P2>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2))
    }
    template<typename FUNC,typename P1,typename P2,typename P3>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5, p6))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5, p6, p7))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5, p6, p7, p8))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8,typename P9>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5, p6, p7, p8, p9))
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8,typename P9,typename P10>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
    {
        CATCH_EXCEPTIONS(luabind::call_function<void>(state, func_name, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10))
    }
};




class Script
{
public:
    Script();
    ~Script();

    bool Run(const QString& src);

    Lua_function_checking_errors callFunction;

//private:
    lua_State* threadState;
    std::string scriptName;
};

#endif // SCRIPT_H


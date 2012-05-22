#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

extern "C" {
    #include <Scripts/lua/lua.h>
    #include <Scripts/lua/lualib.h>
    #include <Scripts/lua/lauxlib.h>
}

#include <Scripts/luabind/luabind.hpp>
#include <Scripts/luabind/operator.hpp>
#include <Scripts/luabind/adopt_policy.hpp>

class ScriptManager
{
public:
    ScriptManager();
    void Initialize();
    void Release();

    inline lua_State* GetState() const {return mainState;}

    static ScriptManager* Get() { return singleton;}

private:
    static ScriptManager* singleton;
    lua_State* mainState;
};

#endif // SCRIPTMANAGER_H

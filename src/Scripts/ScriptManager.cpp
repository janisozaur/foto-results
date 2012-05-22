#include "ScriptManager.h"
#include "Script.h"

ScriptManager* ScriptManager::singleton;

ScriptManager::ScriptManager()
{
    setlocale(LC_ALL,"C");
}

void ScriptManager::Initialize() {
    singleton = this;

    mainState = lua_open();

    luabind::open(mainState);

    if(mainState) {
        luaL_openlibs(mainState);
    }

}

void ScriptManager::Release() {
    lua_close(mainState);
    singleton = NULL;
}


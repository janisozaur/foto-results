#include "Script.h"
#include "ScriptManager.h"
#include <QDebug>

#define AUTO_ERROR_LOGGING

static void stackDump (lua_State *L) {
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;

        }
        printf("  ");  /* put a separator */
      }
      printf("\n");  /* end the listing */
    }



Script::Script() {

    lua_State* masterState = ScriptManager::Get()->GetState();

    // create a thread/state for this object
    threadState = lua_newthread(masterState);

    lua_pushlightuserdata(masterState, threadState);
    lua_pushlightuserdata(masterState, this );
    lua_settable(masterState, LUA_GLOBALSINDEX );

    lua_pop(masterState, 1);

    callFunction.state = threadState;
    //callFunction.function.bind_script(threadState);
    //OOLUA::setup_user_lua_state(threadState);


    lua_pushthread(threadState);
    //thread


    lua_newtable( threadState );
    //thread NewTable


    lua_pushvalue( threadState, -1 );
    //thread NewTable NewTable


    lua_setmetatable( threadState, -2 );
    //thread NewTable

    lua_pushvalue( threadState, LUA_GLOBALSINDEX );
    //thread NewTable GloablTable

    lua_setfield( threadState, -2, "__index" ); //NewTable[__index] = GlobalTable
    //thread NewTable

    lua_setfenv( threadState, -2 ); //thread environment = NewTable
    //thread

    lua_pop(threadState, 1);
    //remove thread from stack

    luabind::set_pcall_callback(PrintLastError);
}

int PrintLastError(lua_State* L)
{
   /*lua_Debug d;

   if(lua_getstack(L, 1, &d)) {
        lua_getinfo(L, "Sln", &d);

        qDebug()<<"Script error"<<d.short_src<<d.currentline;
       // Logger::Log(LOG_ERROR, "Scipt error: %s line %d", d.short_src, d.currentline);
        if (d.name != 0)
        {
            qDebug()<<d.namewhat<<d.name<<d.what;
            //Logger::Log(LOG_ERROR, "(%s %s)", d.namewhat, d.name);
        }
   }*/


   std::string err = lua_tostring(L, -1);
   lua_pop(L, 1);

   qDebug()<<(err.c_str());


   return 0;
}

Script::~Script() {
    lua_gc(threadState, LUA_GCCOLLECT, 0);

    lua_State* masterState = ScriptManager::Get()->GetState();
    lua_pushlightuserdata(masterState, threadState);
    lua_pushlightuserdata(masterState, NULL );
    lua_settable(masterState, LUA_GLOBALSINDEX );
    threadState = NULL;
}

bool Script::Run(const QString& src) {

    int res = luaL_loadbuffer(threadState, src.toStdString().c_str(), src.length(), "code chunk");
    if(res!=0) {
        PrintLastError(threadState);
        return false;
    }

    res = luabind::detail::pcall(threadState, 0, LUA_MULTRET);
    if(res != 0) {
        PrintLastError(threadState);
        return false;
    }


    return true;
}

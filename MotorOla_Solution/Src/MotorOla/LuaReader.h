#pragma once
#include <string>

extern "C"
{
    struct lua_State;
    /*
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
    */
}




/*
* 
player = {
    pos = {
         X = 20,
         Y = 30,
    },
    filename = "res/images/player.png",
    HP = 20,
-- you can also have comments
}
With a little class(implementation is below) you can get data in this way:

LuaScript script("player.lua");
std::string filename = script.get("player.filename");
int posX = script.get("player.pos.X");

*/

void readFile(std::string file);
void readFileMenus(std::string file,const char* get);

void PrintTable(lua_State* L);

void readFileTest(std::string file);

class Entidad;
Entidad* readPrefab(std::string file);

class LuaScript
{
public:
	LuaScript(const std::string&  filename);
	~LuaScript();

	void printError(const std::string& variableName, const std::string& reason);

    template<typename... Targs>
    void callFunction(std::string functionName , int nArg, Targs... args);

    template<typename T>
    T get(const std::string& variableName);

    bool lua_gettostack(const std::string& variableName);

    // Generic get
    template<typename T>
    T lua_get(const std::string& variableName) {
        return 0;
    }


    // Generic default get
    template<typename T>
    T lua_getdefault() {
        return 0;
    }
   

private:
    lua_State* L;
    int level = 0;

    //Motor* m = nullptr;
    //static void exit(Motor* m);

};



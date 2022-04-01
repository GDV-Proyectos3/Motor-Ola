#pragma once
#include <string>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

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

class LuaScript
{
public:
	LuaScript(char*  filename);
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

};

template<>
inline std::string LuaScript::lua_getdefault() {
    return "null";
}

template <>
inline float LuaScript::lua_get(const std::string& variableName) {
    if (!lua_isnumber(L, -1)) {
        printError(variableName, "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get(const std::string& variableName) {
    if (!lua_isnumber(L, -1)) {
        printError(variableName, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get(const std::string& variableName) {
    std::string s = "null";
    if (lua_isstring(L, -1)) {
        s = std::string(lua_tostring(L, -1));
    }
    else {
        printError(variableName, "Not a string");
    }
    return s;
}


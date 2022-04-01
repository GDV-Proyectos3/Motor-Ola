#include "LuaReader.h"
#include <iostream>


LuaScript::LuaScript(char*  filename)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
		L = 0;
	}
		
}

LuaScript::~LuaScript()
{
	if (L) 
		lua_close(L);
}

void LuaScript::printError(const std::string& variableName, const std::string& reason)
{
	std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
}

bool LuaScript::lua_gettostack(const std::string& variableName)
{
    int level = 0;
    std::string var = "";
    for (unsigned int i = 0; i < variableName.size(); i++) {
        if (variableName.at(i) == '.') {
            if (level == 0) {
                lua_getglobal(L, var.c_str());
            }
            else {
                lua_getfield(L, -1, var.c_str());
            }

            if (lua_isnil(L, -1)) {
                printError(variableName, var + " is not defined");
                return false;
            }
            else {
                var = "";
                level++;
            }
        }
        else {
            var += variableName.at(i);
        }
    }
    if (level == 0) {
        lua_getglobal(L, var.c_str());
    }
    else {
        lua_getfield(L, -1, var.c_str());
    }
    if (lua_isnil(L, -1)) {
        printError(variableName, var + " is not defined");
        return false;
    }

    return true;
}

template<typename ...Targs>
void LuaScript::callFunction(std::string functionName,int nArg, Targs ...args)
{
    lua_getglobal(L, functionName.c_str());
    for (int i = 0; i < nArg; i++)
    {
        lua_pushnumber(L, args[i]);
    }
    if (lua_pcall(L, nArg, 1, 0) != 0)
        printError(functionName, "function is not defined");

}

template<typename T>
inline T LuaScript::get(const std::string& variableName)
{
	if (!L) {
		printError(variableName, "Script is not loaded");
		return lua_getdefault<T>();
	}

	T result;

    if (lua_gettostack(variableName)) { // variable succesfully on top of stack
        result = lua_get<T>(variableName);
    }
    else {
        result = lua_getdefault<T>();
    }

    lua_pop(L, level + 1); // pop all existing elements from stack
    return result;
}


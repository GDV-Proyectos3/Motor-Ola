#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include <string>

extern "C"
{
    struct lua_State;
}
class LuaScript
{
public:
    LuaScript(const std::string& filename);
    ~LuaScript();

    void printError(const std::string& variableName, const std::string& reason);

    template<typename... Targs>
    void callFunction(std::string functionName, int nArg, Targs... args);

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

};

MOTOR_API void readFile(std::string file);
MOTOR_API void readFileMenus(std::string file,const char* get);

void PrintTable(lua_State* L);

void readFileTest(std::string file);

class Entidad;
MOTOR_API Entidad* readPrefab(std::string file);





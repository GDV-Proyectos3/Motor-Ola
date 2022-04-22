#pragma once
#include <string>
extern "C"
{
    struct lua_State;
   
}





void readFile(std::string file);

void PrintTable(lua_State* L);

void readFileTest(std::string file);

class Entidad;
Entidad* readPrefab(std::string file);



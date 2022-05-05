#include "LuaReader.h"
#include <vector>
#include <iostream>
#include <map>

// Clases propias
#include "Entidad.h"
#include "utils/Singleton.h"
#include "EntidadManager.h"
#include "OgreManager.h"
#include "OverlayManager.h"
#include <stdio.h>
#include "PhysxManager.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

static const luaL_Reg lualibs[] = {
		{ "base",       luaopen_base },
		{ NULL,         NULL }
};

void openlualibs(lua_State* l) {
	const luaL_Reg* lib;

	for (lib = lualibs; lib->func != NULL; lib++) {
		lib->func(l);
		lua_settop(l, 0);
	}
}

void readFile(std::string file) {
	// Vector de entidades que creamos y vector auxiliar para marcarlas iniciadas
	std::vector<Entidad*> ents;
	std::vector<bool> entInits;

	// Preparamos un LuaState para leer el fichero
	lua_State* l;
	l = luaL_newstate();
	openlualibs(l);

#if _DEBUG
	std::printf("now calling lua\n\n");
#endif
	// Intenta abrir el archivo .lua
	if (!luaL_loadfile(l, file.c_str()) && lua_pcall(l, 0, 0, 0)) {
		std::cout << lua_tostring(l, -1) << "\n";
		std::cout << "Error reading .lua\n";
		throw std::exception("Lua file was not able to be loaded\n");
	}

	// Intenta leer la escena
	try {
		std::cout << "LuaGettingLevel "  << lua_getglobal(l, "GetLevel") << "\n";

		// Si no encuentra la function que devuelve la tabla
		if (lua_pcall(l, 0, 1, 0) != LUA_OK) {
			std::cout << lua_tostring(l,-1) << "\nError reading GetLevel in .lua\n";	
			throw std::exception("Lua function GetLevel was not able to be loaded");
		}

		// Primero asigna el color de fondo de la escena
		lua_getfield(l, -1, "backgroundColor");
		std::string aux = lua_tostring(l, -1);
		std::string::size_type sz = 0, sa = 0;
		float a = std::stof(aux, &sz), b = std::stof(aux.substr(sz + 1), &sa), c = std::stof(aux.substr(sz + sa + 2));
		Singleton<OgreManager>::instance()->getViewPort()->setBackgroundColour(Ogre::ColourValue(a, b, c, 1.0f));
		lua_pop(l, 1);

		// Luego la luz ambiente
		lua_getfield(l, -1, "ambient");
		std::string aux2 = lua_tostring(l, -1);
		sz = 0, sa = 0;
		a = std::stof(aux2, &sz); b = std::stof(aux2.substr(sz + 1), &sa); c = std::stof(aux2.substr(sz + sa + 2));
		Singleton<OgreManager>::instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(a, b, c));
		lua_pop(l, 1);

		// Modifica la gravedad de la escena
		lua_getfield(l, -1, "gravity");
		std::string aux3 = lua_tostring(l, -1);
		sz = 0, sa = 0;
		a = std::stof(aux3, &sz); b = std::stof(aux3.substr(sz + 1), &sa); c = std::stof(aux3.substr(sz + sa + 2));
		pm().getScene()->setGravity(PxVec3(a, b, c));

		lua_pop(l, 1);

		// Modifica la camara de la escena
		lua_getfield(l, -1, "camera");
		//lua_pushnil(l);

		// Obtenemos la camara y su nodo
		Ogre::Camera* cam = Singleton<OgreManager>::instance()->getCam();
		Ogre::SceneNode* camNode = Singleton<OgreManager>::instance()->getCamNode();

		// Ajustamos sus parametros
		lua_getfield(l, -1, "nearClipDistance");
		int nCD = lua_tonumber(l, -1);
		cam->setNearClipDistance(nCD);
		lua_pop(l, 1);

		lua_getfield(l, -1, "farClipDistance");
		int fCD = lua_tonumber(l, -1);
		cam->setFarClipDistance(fCD);
		lua_pop(l, 1);

		lua_getfield(l, -1, "camPosition");
		sz = 0, sa = 0;
		std::string camPos = lua_tostring(l, -1);
		float cx, cy, cz;
		cx = stof(camPos, &sz);
		std::string temp = camPos.substr(sz + 1);
		cy = stof(temp, &sa);
		cz = stof(camPos.substr(sz + sa + 2));
		camNode->setPosition(cx, cy, cz);
		////////////////camNode->setPosition(0, -800, 1200);////////_test_caidas_forma_javi_////////
		lua_pop(l, 1);

		lua_getfield(l, -1, "camRotation");
		sz = 0; sa = 0;
		std::string camRot = lua_tostring(l, -1);
		cx = stof(camRot, &sz);
		temp = camRot.substr(sz + 1);
		cy = stof(temp, &sa);
		cz = stof(camRot.substr(sz + sa + 2));
		camNode->lookAt(Ogre::Vector3(cx, cy, cz), Ogre::Node::TS_WORLD);
		lua_pop(l, 1);	

		Singleton<OgreManager>::instance()->getRenderWindow()->getViewport(0)->update();

		lua_pop(l, 1);

		// Después lee todas las entidades y los componentes de cada una
		lua_getfield(l, -1, "entidades");
		lua_pushnil(l);
		while (lua_next(l, 2) != 0) {
			// Entity is here
			// Name
			lua_getfield(l, -1, "name");
			char* name = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			// ID
			lua_getfield(l, -1, "id");
			int id = lua_tonumber(l, -1);
			lua_pop(l, 1);

			Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad(name, id);
			ents.push_back(ent);
			entInits.push_back(false);

			// Components
			// Calls a similar while loop, creating a set<string, string> with each pair
			// Knows which component is by key name and a translator function
			lua_getfield(l, -1, "components");
			lua_pushnil(l);
			while (lua_next(l, 5) != 0) { // stack: mapa-entities-indEntity-Entity-compTabla

				char* compName = (char*)lua_tostring(l, -2);

				std::map<std::string, std::string> compMap;
				lua_pushnil(l);
				while (lua_next(l, 7) != 0) { // stack: mapa-entities-indEntity-Entity-compTabla-indComp-Component
					char* attrName = (char*)lua_tostring(l, -2);
					std::string s1(attrName);
					char* attrValue = (char*)lua_tostring(l, -1);
					std::string s2(attrValue);
					compMap.insert((std::pair<std::string, std::string>(s1, s2)));
					lua_pop(l, 1);
				}

				// Funcion de traduccion
				ent->addComponent(compName, compMap);
				lua_pop(l, 1);
			}

			lua_pop(l, 1);
			// Entity is no longer here, only key to be removed by lua_next
			lua_pop(l, 1);
		}

		lua_pop(l, 2);
#if _DEBUG
		std::printf("\ndo something else\n\n");
#endif
		lua_close(l);

		int initTotales = 0;
		int i = 0;
		int numEnts = ents.size();
		int initedEnts = 0;
		while (initedEnts != numEnts) {
			if (!entInits[i] && ents[i]->init()) {
				++initedEnts;
				entInits[i] = true;
				std::cout << "Entidad " << i << " iniciada\n";
				initTotales++;
			}
			++i;
			i %= numEnts;
		}

		std::cout << "Init Totales "<< initTotales << "\n";
	}
	catch (...) {
		throw std::exception("Lua file has incorrect formatting\n");
	}
}

void readFileMenus(std::string file,const char* get)
{

	// Preparamos un LuaState para leer el fichero
	lua_State* l;
	l = luaL_newstate();
	openlualibs(l);


	
	if (!luaL_loadfile(l, file.c_str()) && lua_pcall(l, 0, 0, 0)) {
		std::cout << lua_tostring(l, -1) << "\n";
		std::cout << "Error reading .lua\n";
		throw std::exception("Lua file was not able to be loaded\n");
	}

	// Intenta leer la escena
	try {
		std::cout << "LuaGettingLevel " << lua_getglobal(l, get) << "\n";

		// Si no encuentra la function que devuelve la tabla
		if (lua_pcall(l, 0, 1, 0) != LUA_OK) {
			std::cout << lua_tostring(l, -1) << "\nError reading GetLevel in .lua\n";
			throw std::exception("Lua function GetLevel was not able to be loaded");
		}

		// Primero asigna el color de fondo de la escena
		lua_getfield(l, -1, "backgroundColor");
		std::string aux = lua_tostring(l, -1);
		std::string::size_type sz = 0, sa = 0;
		float a = std::stof(aux, &sz), b = std::stof(aux.substr(sz + 1), &sa), c = std::stof(aux.substr(sz + sa + 2));
		Singleton<OgreManager>::instance()->getViewPort()->setBackgroundColour(Ogre::ColourValue(a, b, c, 1.0f));
		lua_pop(l, 1);

		// Luego la luz ambiente
		lua_getfield(l, -1, "ambient");
		std::string aux2 = lua_tostring(l, -1);
		sz = 0, sa = 0;
		a = std::stof(aux2, &sz); b = std::stof(aux2.substr(sz + 1), &sa); c = std::stof(aux2.substr(sz + sa + 2));
		Singleton<OgreManager>::instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(a, b, c));
		lua_pop(l, 1);

		// Modifica la gravedad de la escena
		lua_getfield(l, -1, "gravity");
		aux = lua_tostring(l, -1);
		//Singleton<Physx> cambiar gravedad
		lua_pop(l, 1);

		// Después lee todas las entidades y los componentes de cada una
		lua_getfield(l, -1, "botones");
		lua_pushnil(l);
		while (lua_next(l, 2) != 0) {
			// Entity is here
			// Name
			lua_getfield(l, -1, "positionX");
			float positionX = lua_tonumber(l, -1);
			lua_pop(l, 1);

			// ID
			lua_getfield(l, -1, "positionY");
			float positionY = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "texto");
			char* texto = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "nombrePanel");
			char* nombrePanel = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "nombreTexto");
			char* nombreTexto = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "tamLetra");
			float tamLetra = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "material");
			char* material = (char*)lua_tostring(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "dimensionX");
			float dimensionX = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "dimensionY");
			float dimensionY = lua_tonumber(l, -1);
			lua_pop(l, 1);

			Singleton<OverlayManager>::instance()->creaBoton(positionX, positionY, texto, nombrePanel, nombreTexto, tamLetra, material, dimensionX, dimensionY);
			
			// Entity is no longer here, only key to be removed by lua_next
			lua_pop(l, 1);
		}

		lua_pop(l, 2);
#if _DEBUG
		std::printf("\ndo something else\n\n");
#endif
		lua_close(l);
	}
	catch (...) {
		throw std::exception("Lua file has incorrect formatting\n");
	}
}

// In Lua 5.0 reference manual is a table traversal example at page 29.
void PrintTable(lua_State* L)
{
	lua_pushnil(L);

	while (lua_next(L, -2) != 0)
	{
		if (lua_isstring(L, -1))
			printf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
		else if (lua_isnumber(L, -1))
			printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
		else if (lua_istable(L, -1))
			PrintTable(L);

		lua_pop(L, 1);
	}
}

void readFileTest(std::string file)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	// Load file.
	if (luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		printf("Cannot run file\n");
		return;
	}

	// Print table contents.
	lua_getglobal(L, "level1");
	PrintTable(L);

	// Print music field.
	lua_getglobal(L, "level1");
	lua_pushstring(L, "music");
	lua_gettable(L, -2);

	if (lua_isstring(L, -1))
		printf("\nlevel1.music = %s\n", lua_tostring(L, -1));

	lua_close(L);
}

Entidad* readPrefab(std::string file) {
	lua_State* l;
	l = luaL_newstate();
	openlualibs(l);
#if _DEBUG
	std::printf("now calling lua\n\n");
#endif
	if (!luaL_loadfile(l, file.c_str()) && lua_pcall(l, 0, 0, 0)) {
		throw std::exception("Lua file was not able to be loaded");
	}
	try {
		lua_getglobal(l, "GetPrefab");
		int err = lua_pcall(l, 0, 1, 0); // GetPrefab()

		// Entity is here
		// Name
		lua_getfield(l, -1, "name");
		char* name = (char*)lua_tostring(l, -1);
		lua_pop(l, 1);

		// ID
		lua_getfield(l, -1, "id");
		int id = lua_tonumber(l, -1);
		lua_pop(l, 1);

		// showCursor
		lua_getfield(l, -1, "cursor");
		int cursor = lua_tonumber(l, -1);
		lua_pop(l, 1);
		

		Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad(name, id);

		// Components
		// Calls a similar while loop, creating a set<string, string> with each pair
		// Knows which component is by key name and a translator function
		lua_getfield(l, -1, "components");
		lua_pushnil(l);
		while (lua_next(l, 2) != 0) { // stack: Entity-compTabla

			char* compName = (char*)lua_tostring(l, -2);

			std::map<std::string, std::string> compMap;
			lua_pushnil(l);
			while (lua_next(l, 4) != 0) { // stack: Entity-compTabla-indComp-Component
				char* attrName = (char*)lua_tostring(l, -2);
				std::string s1(attrName);
				char* attrValue = (char*)lua_tostring(l, -1);
				std::string s2(attrValue);
				compMap.insert((std::pair<std::string, std::string>(s1, s2)));
				lua_pop(l, 1);
			}

			// Funcion de traduccion
			ent->addComponent(compName, compMap);
			lua_pop(l, 1);
		}

		ent->init();

		lua_pop(l, 1);
		// Entity is no longer here, only key to be removed by lua_next
		return ent;
	}
	catch (...) {
		throw std::exception("Prefab file has incorrect formatting");
	}
}


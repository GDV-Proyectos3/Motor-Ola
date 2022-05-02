#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <map>
#include <string>
#include "utils/Singleton.h"
//#include <experimental/filesystem>
#include <filesystem>
//using namespace std::experimental::filesystem;
using namespace std::filesystem;
using namespace std;
//using namespace filesystem;
//static const std::string ASSETS = "../../Exes/Assets/";
static const std::string ASSETS = "../../Exes/Assets";


MOTOR_API class LoadResources : public Singleton<LoadResources> {
	friend Singleton<LoadResources>;
public:
	LoadResources();
	~LoadResources();
	MOTOR_API void init();
	MOTOR_API string mes(string m);
	MOTOR_API string aud(string name);
	MOTOR_API string tex(string tex);
	MOTOR_API string scene(string scene);
	MOTOR_API string prefab(string prefab);
private:
	map<string, string>mesh;//.mesh
	map<string, string>audio;//.mp3,.ogg,.wav
	map<string, string>textures;//.png,.jpg,.bmp
	map<string, string>scenes;//.lua
	map<string, string>prefabs;
	//Si hay otro tipo añadir el map
	void search(path p);
	void load(path p, size_t end, size_t pathLenght);
};


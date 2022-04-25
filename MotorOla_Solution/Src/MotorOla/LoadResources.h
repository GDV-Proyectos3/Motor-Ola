#pragma once
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
class LoadResources : public Singleton<LoadResources> {
	friend Singleton<LoadResources>;
public:
	LoadResources();
	~LoadResources();
	void init();
	string mes(string m);
    string aud(string name);
	string tex(string tex);
	string scene(string scene);
	string prefab(string prefab);
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


#include "LoadResources.h"

std::unique_ptr<LoadResources> Singleton<LoadResources>::instance_ = nullptr;

LoadResources::LoadResources()
{
}

LoadResources::~LoadResources()
{
}

void LoadResources::init()
{
	search(ASSETS);
}

string LoadResources::mes(string m)
{
	auto it = mesh.find(m);
	return it->second;
}

string LoadResources::aud(string name)
{
	auto s = audio.find(name);
	return s->second;
}

string LoadResources::tex(string tex)
{
	auto t = textures.find(tex);
	return t->second;
}

string LoadResources::scene(string scene)
{
	auto it = scenes.find(scene);
	return it->second;
}

string LoadResources::prefab(string prefab)
{
	auto it = prefabs.find(prefab);
	return it->second;
}

void LoadResources::search(path p)
{
	// Recorre los directorios cargando los archivos en los diferentes map
	for (const auto& it : directory_iterator(p)) {
		std::cout << "\n";
		size_t end = it.path().string().find_last_of(".");
		if (end > 4) {
			load(it.path(), end, p.string().length());
		}
		else {
			search(it.path());
		}
	}
	
}

void LoadResources::load(path p, size_t end, size_t pathLenght)
{
	//Cargan los assets cada uno en el map que le corresponde 
	//Añadir si hay otro tipo de assets
	string extension = p.string().substr(end);
	string name = p.string().substr(pathLenght + 1);
	if (extension == ".mp3" || extension == ".ogg" || extension == ".wav") {
		audio.insert(pair<string, string>(name, p.string()));
	}
	else if (extension == ".jpg" || extension == ".png" || extension == ".bmp") {
		textures.insert(pair<string, string>(name, p.string()));
	}
	else if (extension == ".mesh") {
		mesh.insert(pair<string, string>(name, p.string()));
	}
	else if (extension == ".lua") {
		scenes.insert(std::pair<std::string, std::string>(name, p.string()));
	}
	else if (extension == ".prefab") {
		prefabs.insert(std::pair<std::string, std::string>(name, p.string()));
	}
}

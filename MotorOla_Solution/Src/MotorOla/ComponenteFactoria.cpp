#include "ComponenteFactoria.h"

std::unique_ptr<ComponenteFactoria> Singleton<ComponenteFactoria>::instance_ = nullptr;

Componente* ComponenteFactoria::getComponent(std::string name)
{
	auto it = _mGenerators.find(name);
	if (it != _mGenerators.end())
	{
		return it->second();
	}
	return nullptr;
}

bool ComponenteFactoria::registerGenerator(std::string compName, const componentInstanceGenerator& instGenerator)
{
	return _mGenerators.insert(std::make_pair(compName, instGenerator)).second;
}
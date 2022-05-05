#pragma once
#include "Componente.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

class Mesh : public Componente
{
public:
	Mesh();
	virtual ~Mesh();

	// Función obligatoria para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa) override;

	bool  getActive();
	void  setActive(bool a) ;
	Ogre::SceneNode* getNodo() ;

	void setMesh(std::string mesh);
	void setMaterial(std::string material);

	void update();

	/// <summary>
	/// Devuelve si es o no visible la entidad
	/// </summary>
	inline bool isVisible() { return _visible; };

	/// <summary>
	/// Hace visible a la entidad
	/// </summary>
	void setVisible(bool visible);

private:
	bool _visible = true;
	bool _active = true;
	Ogre::SceneManager* _sceneManager = nullptr;
	Ogre::SceneNode* _nodo = nullptr;
	Ogre::Entity* _ogreEntity = nullptr;
};


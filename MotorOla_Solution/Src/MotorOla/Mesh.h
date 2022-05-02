#pragma once
#include "Componente.h"
#include "./utils/Vector2D.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "utils/Vector2D.h"
#include <OgreParticleSystem.h>

class Mesh : public Componente
{
public:
	Mesh();

	//Mesh(Ogre::SceneNode* node);
	//Mesh(Ogre::SceneNode* node, std::string mesh, std::string material);
	virtual ~Mesh();

	// Función obligatoria para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa) override;

	bool  getActive();
	void  setActive(bool a) ;
	Ogre::SceneNode* getNodo() ;

	void setMesh(std::string mesh);
	void setMaterial(std::string material);

	//void setBillboard(std::string material , Vector2D pos, Vector2D scale);
	//void toggleBillboard();
	//void setParticle(std::string material);
	//void toggleParticle();
	//void setLight();
	//void toggleLight();

	void update();
	//void draw();

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

	bool debugCom = false;
};


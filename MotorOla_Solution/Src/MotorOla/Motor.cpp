#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "utils/Singleton.h"
#include "InputManager.h"
#include "Mesh.h"
#include "Transform.h"
#include "Entidad.h"
#include "EntidadManager.h"
#include "Movible.h"
#include "Pala.h"
#include <Windows.h>
#include "AudioSource.h"
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreVector.h>
#include "LoadResources.h"
#include "FMODAudioManager.h"


typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);


Motor::Motor()
{
	// Inicia los managers
	if (!_loadResources)_loadResources = new LoadResources();
	if(!_ogreManager) _ogreManager = new OgreManager();	
	if (!_inputManager)_inputManager = new InputManager();
	if (!_entidadManager) _entidadManager = new EntidadManager();
	if (!_audioManager)_audioManager = new FMODAudioManager();
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
	if (_inputManager)delete _inputManager;
	if (_loadResources)delete _loadResources;
	if (_audioManager)delete _audioManager;
	if (_entidadManager) delete _entidadManager;
}

void Motor::initSystems()
{

	_loadResources->init();
	_ogreManager->init();
	_inputManager->init(this);
	_audioManager->init();
	/*_audioManager->init();
	_audioManager->loadMusic(0, _loadResources->aud("blind_shift.mp3").c_str());
	_audioManager->playMusic(0, true);*/
	
	// Registrando Componentes
	registryComponents();

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		//loadTestMotorGame();
		loadPong();
	}
}

void Motor::registryComponents()
{
	ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
	ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	ComponenteRegistro::ComponenteRegistro<Movible>("movible");
	ComponenteRegistro::ComponenteRegistro<Pala>("pala");

}

void Motor::mainLoop()
{
	std::cout << "------------------- COMIENZA EL BUCLE PRINCIPAL -------------------\n";
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		// Recoger el Input
		_inputManager->handleEvents();

		// Actualizar las físicas de las entidades
		//_physxManager->update();

		// Actualiza los transforms de las entitys después de las físicas

		// Actualiza el resto de componentes (también los del juego)
		_entidadManager->update();

		// Actualiza los transforms de las entitys de Ogre
		//for (auto e = _entidadManager->entitys_.begin(); e  != _entidadManager->entitys_.end(); e++)
		//{
		//	
		//}

		//Ogre::Vector3 v;
		////v.x = ent3->getComponent<Transform>()->getPos().getX();
		////v.y = ent3->getComponent<Transform>()->getPos().getY();
		////v.z;		
		//v.x = ent2->getParentNode()->getPosition().x + 0.1;
		//v.y = ent2->getParentNode()->getPosition().y;
		//v.z = ent2->getParentNode()->getPosition().z;

		//ent2->getParentNode()->setPosition(v);

		// Renderiza las entidades
		_ogreManager->update();


		//std::cout << "Vuelta " << ++frame << "\n";
	}
}

void Motor::loadDLLGame()
{
	HINSTANCE hDLL;               // Handle to DLL
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	HRESULT hrReturnVal;

	hDLL = LoadLibrary(L"..\\GameToLoad\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char

	if (NULL != hDLL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw "Function LoadGame not found in DLL";

		// Libera la memoria de la DLL cargada explicitamente
		FreeLibrary(hDLL);
	}
	else throw "DLL not found";
}

void Motor::loadTestMotorGame() 
{
	// Audio
	Entidad* ent = _entidadManager->addEntidad();
	ent->addComponent<AudioSource>(channel, _audioManager, _loadResources->aud("blind_shift.mp3").c_str());
	if (ent->hasComponent<AudioSource>()) {
		ent->getComponent<AudioSource>()->play();
	}

	// Cabeza
	Entidad* entMesh = _entidadManager->addEntidad();
	entMesh->addComponent<Transform>();
	Ogre::SceneNode* nodo = _ogreManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("NodoCabeza");
	entMesh->addComponent<Mesh>(nodo, "ogrehead.mesh", "material");
	entMesh->addComponent<Movible>();

	//Entidad* entNoMesh = _entidadManager->addEntidad();
	//entNoMesh->addComponent<Transform>();
	//Ogre::SceneNode* nodo2 = _ogreManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("NodoCabeza2");
	//Ogre::Entity* ent2 = _ogreManager->getSceneManager()->createEntity("ogrehead.mesh");
	//nodo2->attachObject(ent2);

	std::cout << nodo->getPosition() << nodo->getScale() << endl;
	//std::cout << nodo2->getPosition() << nodo2->getScale() << endl;
}

void Motor::loadPong() {
	// Pala 1
	Entidad* pala1 = _entidadManager->addEntidad();
	pala1->addComponent<Transform>();
	Ogre::SceneNode* nodoPala1 = _ogreManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("nodoPala1");
	pala1->addComponent<Mesh>(nodoPala1, "cube.mesh", "material");
	pala1->addComponent<Pala>();

	pala1->getComponent<Transform>()->setH(1.5);
	pala1->getComponent<Transform>()->setW(0.3);

	_inputManager->setPala1(pala1);
}


bool Motor::getStop()
{
	return stop;
}

void Motor::setStop(bool s)
{
	stop = s;
}


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
#include <Windows.h>
#include "AudioSource.h"
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreVector.h>


typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

#include "LoadResources.h"
#include "FMODAudioManager.h"

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
	ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
	ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	ComponenteRegistro::ComponenteRegistro<Movible>("movible");

	
	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		// Recoger el Input
		_inputManager->handleEvents();

		// Actualizar las físicas de las entidades
		//_physxManager->update();


		// Actualiza el resto de componentes (también los del juego)
		_entidadManager->update();

		// Actualiza los transforms de las entitys de Ogre
		//for (auto e = _entidadManager->entitys_.begin(); e  != _entidadManager->entitys_.end(); e++)
		//{
		//	
		//}

		Ogre::Vector3 v;
		//v.x = ent3->getComponent<Transform>()->getPos().getX();
		//v.y = ent3->getComponent<Transform>()->getPos().getY();
		//v.z;		
		v.x = ent2->getParentNode()->getPosition().x + 0.1;
		v.y = ent2->getParentNode()->getPosition().y;
		v.z = ent2->getParentNode()->getPosition().z;

		ent2->getParentNode()->setPosition(v);

		// Renderiza las entidades
		_ogreManager->update();
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
	// audio
	Entidad* ent = _entidadManager->addEntidad();
	ent->addComponent<AudioSource>(channel, _audioManager, _loadResources->aud("blind_shift.mp3").c_str());
	if (ent->hasComponent<AudioSource>()) {
		ent->getComponent<AudioSource>()->play();
	}

	// cabeza
	ent3 = _entidadManager->addEntidad();
	ent3->addComponent<Transform>();
	Ogre::SceneNode* cuerpoNode = _ogreManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("NodoPrueba");
	ent2 = _ogreManager->getSceneManager()->createEntity("ogrehead.mesh");
	
	//ent3->addComponent<Mesh>(cuerpoNode, "ogrehead.mesh", "");
	cuerpoNode->createChildSceneNode();
	cuerpoNode->attachObject(ent2);
	cuerpoNode->setScale(1.5, 1.5, 1.5); //sc

	ent3->addComponent<Movible>();
}


bool Motor::getStop()
{
	return stop;
}

void Motor::setStop(bool s)
{
	stop = s;
}


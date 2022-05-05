#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "utils\Singleton.h"

#include <string>

#include <Ogre.h>
#include <OgreBuildSettings.h>
#include <OgreLogManager.h>
#include <OgrePlugin.h>
#include <OgreFileSystemLayer.h>
#include <OgreFrameListener.h>
#include <OgreOverlayPrerequisites.h> 
#include <SDL.h> 

namespace Ogre {
	class FileSystemLayer;
	class OverlaySystem;
	class Root;
	class RenderWindow;
	class SceneManager;
}

typedef SDL_Window NativeWindowType;

struct NativeWindowPair
{
	Ogre::RenderWindow* render = nullptr;
	NativeWindowType* native = nullptr;
};

//class OgreManager : public Singleton<OgreManager>, Ogre::FrameListener {	// Creo que esto va a ser InputManager
MOTOR_API class OgreManager : public Singleton<OgreManager> {
	friend Singleton<OgreManager>;
public:
	/// <summary>
	/// Inicia los parámetros de la clase
	/// </summary>
	/// <param name="appName"> Nombre que aparecerá en la ventana (el del juego) </param>
	OgreManager(const Ogre::String& appName = "MotorOla");

	/// <summary>
	/// Destructor de la clase
	/// </summary>
	~OgreManager();

	// Getters
	MOTOR_API Ogre::Camera* getCam() const { return cam; };
	MOTOR_API Ogre::SceneNode* getCamNode() const { return camNode; };
	MOTOR_API Ogre::RenderWindow* getRenderWindow() const { return _window.render; }
	MOTOR_API Ogre::SceneManager* getSceneManager() const { return _sceneManager; }
	MOTOR_API Ogre::Viewport* getViewPort() const { return _vp; }
	MOTOR_API Ogre::Root* getRoot() const { return _root; }
	MOTOR_API Ogre::OverlaySystem* getOverlaySystem() const { return _overlaySystem; }
	MOTOR_API int getWindowWidth() const { return _window.render->getWidth(); }
	MOTOR_API int getWindowHeight() const { return _window.render->getHeight(); }

	MOTOR_API void init();
	MOTOR_API void update();
	MOTOR_API void close();

	// callback interface copied from various listeners to be used by ApplicationContext
	
	MOTOR_API virtual void windowMoved(Ogre::RenderWindow* rw) {}
	MOTOR_API virtual void windowResized(Ogre::RenderWindow* rw) {}
	MOTOR_API virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	MOTOR_API virtual void windowClosed(Ogre::RenderWindow* rw) {}
	MOTOR_API virtual void windowFocusChange(Ogre::RenderWindow* rw) {}

	// Funciones

	MOTOR_API void setup();
	MOTOR_API void createRoot();
	MOTOR_API bool oneTimeConfig();
	MOTOR_API void setWindowGrab(bool grab);
	MOTOR_API void locateResources();
	MOTOR_API void loadResources();
	MOTOR_API void shutdown();

	

	MOTOR_API virtual NativeWindowPair createWindow(const Ogre::String& name);
private:
	// Variables
	Ogre::Root* _root = nullptr;
	NativeWindowPair _window;
	Ogre::FileSystemLayer* _fileSystemLayer = nullptr;
	Ogre::OverlaySystem* _overlaySystem=nullptr;  // Overlay system
	Ogre::SceneManager* _sceneManager = nullptr;
	Ogre::Viewport* _vp = nullptr;
	Ogre::Camera* cam = nullptr;
	Ogre::SceneNode* camNode = nullptr;
	bool _firstRun;
	Ogre::String _appName = "MotorOla";
	Ogre::String _solutionPath;

};


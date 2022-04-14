#pragma once

#include "utils\Singleton.h"

#include <string>

#include <Ogre.h>
#include <OgreBuildSettings.h>
#include <OgreLogManager.h>
#include <OgrePlugin.h>
#include <OgreFileSystemLayer.h>
#include <OgreFrameListener.h>
#include <OgreOverlayPrerequisites.h> 
#include <OgreFrameListener.h>
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
class OgreManager : public Singleton<OgreManager> {
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
	Ogre::RenderWindow* getRenderWindow() const { return _window.render; }
	Ogre::SceneManager* getSceneManager() const { return _sceneManager; }
	Ogre::Root* getRoot() const { return _root; }
	Ogre::OverlaySystem* getOverlaySystem() const { return _overlaySystem; }
	


	void init();
	void update();
	void close();

	// callback interface copied from various listeners to be used by ApplicationContext
	//virtual bool frameStarted(const Ogre::FrameEvent& evt) { pollEvents(); return true; }
	//virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	//virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }
	virtual void windowMoved(Ogre::RenderWindow* rw) {}
	virtual void windowResized(Ogre::RenderWindow* rw) {}
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	virtual void windowClosed(Ogre::RenderWindow* rw) {}
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {}

	// Funciones

	//bool initialiseRTShaderSystem();
	//void destroyRTShaderSystem();

	void setup();
	void createRoot();
	bool oneTimeConfig();
	void setWindowGrab(bool grab);
	void locateResources();
	void loadResources();
	void shutdown();

	//void pollEvents();

	virtual NativeWindowPair createWindow(const Ogre::String& name);
private:
	// Variables
	Ogre::Root* _root = nullptr;
	NativeWindowPair _window;
	Ogre::FileSystemLayer* _fileSystemLayer = nullptr;
	Ogre::OverlaySystem* _overlaySystem=nullptr;  // Overlay system
	Ogre::SceneManager* _sceneManager = nullptr;
	bool _firstRun;
	Ogre::String _appName = "MotorOla";
	Ogre::String _solutionPath;

	//Ogre::String _renderStateShaderLibPath;
	//Ogre::RTShader::ShaderGenerator* _shaderGenerator;
	//SGTechniqueResolverListener* _materialMgrListener; // Shader generator material manager listener.
};
/*inline OgreManager& og() {
	return *OgreManager::instance();
}*/

#pragma once
#include "utils\Singleton.h"

#include <Ogre.h>
#include <OgreFrameListener.h>
#include <SDL.h> 

//#include <OgreWindowEventUtilities.h>

//class Singleton;

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

class OgreManager : public Singleton<OgreManager>, Ogre::FrameListener {
	friend Singleton<OgreManager>;
public:
	OgreManager();
	~OgreManager();

	void init();
	void update();

private:
	// Variables
	Ogre::FileSystemLayer* _fileSystemLayer = nullptr;
	Ogre::OverlaySystem* _OverlaySystem = nullptr;
	Ogre::Root* _root = nullptr;
	NativeWindowPair _window;
	Ogre::SceneManager* _sceneManager = nullptr;
	Ogre::String _solutionPath;
	Ogre::String _appName = "MotorOla";
	//Ogre::RTShader::ShaderGenerator* _shaderGenerator;

	// Funciones
	void createRoot();
	void setup();

	virtual NativeWindowPair createWindow(const Ogre::String& name);
	//void setWindowGrab(bool grab);
	virtual void locateResources();
};


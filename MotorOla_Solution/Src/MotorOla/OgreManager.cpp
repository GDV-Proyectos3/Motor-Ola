#include "OgreManager.h"

#include <OgreBuildSettings.h>
#include <OgreLogManager.h>
#include <OgrePlugin.h>
#include <OgreFileSystemLayer.h>
#include <OgreFrameListener.h> 
#include <OgreString.h>

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>
//#include <SDLInputMapping.h>

OgreManager::OgreManager()
{
    std::cout << "CONSTRUCTOR DE OGRE MANAGER\n";
    _fileSystemLayer = new Ogre::FileSystemLayer("MotorOla");
    _root = nullptr;
    //mOverlaySystem = nullptr;
    //mFirstRun = true;

    //mShaderGenerator = nullptr;
    //mMaterialMgrListener = nullptr;
}

OgreManager::~OgreManager()
{
    std::cout << "DESTRUCTOR DE OGRE MANAGER\n";
}

void OgreManager::init()
{
    // Crea el root de Ogre
    createRoot();

    // Inicia la ventana
    setup();

    // Inicia el Scene Manager
    //_sceneManager = _root->createSceneManager();

    //// Crea la ventana de la aplicacion
    //_renderWindow = _root->initialise(true, "MotorOlaWindow");
    ////_renderWindow = _root->createRenderWindow("Window", 800,600,false);

    //// Para empezar a renderizar
    //_root->startRendering();

    //_renderWindow = _root->initialise(true, "Ventana Motor");
    std::cout << "Continua\n";

}

void OgreManager::update()
{
}

void OgreManager::createRoot()
{
    Ogre::String pluginsPath;
    pluginsPath = _fileSystemLayer->getConfigFilePath("plugins.cfg");

    // Se comprueba la existencia de plugins.cfg
    if (!Ogre::FileSystemLayer::fileExists(pluginsPath))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "plugins.cfg", "Error buscando el archivo plugins.cfg");
    }

    _solutionPath = pluginsPath;    // IG2: añadido para definir directorios relativos al de la solución 
    _solutionPath.erase(_solutionPath.find_last_of("\\") + 1, _solutionPath.size() - 1);
    _fileSystemLayer->setHomePath(_solutionPath);   // IG2: para los archivos de configuración ogre. (en el bin de la solubión)
    _solutionPath.erase(_solutionPath.find_last_of("\\") + 1, _solutionPath.size() - 1);   // IG2: Quito /bin

    _root = new Ogre::Root(pluginsPath, _fileSystemLayer->getWritablePath("ogre.cfg"), _fileSystemLayer->getWritablePath("ogre.log"));

    _root->setRenderSystem(_root->getRenderSystemByName("OpenGL Rendering Subsystem"));
}

void OgreManager::setup()
{
    _root->initialise(false);

    createWindow("MotorOla");
    //setWindowGrab(false);

    //locateResources();
    //initialiseRTShaderSystem();
    //loadResources();

    //_root->addFrameListener(this);
    //_root->showConfigDialog(NULL);

    _sceneManager = _root->createSceneManager();
    //_mSM->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    //_mShaderGenerator->addSceneManager(_mSM);
}


NativeWindowPair OgreManager::createWindow(const Ogre::String& name)
{
    uint32_t w, h;
    Ogre::NameValuePairList miscParams;

    Ogre::ConfigOptionMap ropts = _root->getRenderSystem()->getConfigOptions();

    std::istringstream mode(ropts["Video Mode"].currentValue);
    Ogre::String token;
    mode >> w; // width
    mode >> token; // 'x' as seperator between width and height
    mode >> h; // height

    miscParams["FSAA"] = ropts["FSAA"].currentValue;
    miscParams["vsync"] = ropts["VSync"].currentValue;
    miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

    if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);

    Uint32 flags = SDL_WINDOW_RESIZABLE;

    if (ropts["Full Screen"].currentValue == "Yes")  flags = SDL_WINDOW_FULLSCREEN;
    //else  flags = SDL_WINDOW_RESIZABLE;

    _window.native = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(_window.native, &wmInfo);

    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

    _window.render = _root->createRenderWindow(name, w, h, false, &miscParams);
    return _window;
}

//void OgreManager::setWindowGrab(bool _grab)
//{
//    SDL_bool grab = SDL_bool(_grab);
//    SDL_SetWindowGrab(_window.native, grab);
//    //SDL_SetRelativeMouseMode(grab);
//    SDL_ShowCursor(grab);
//}

void OgreManager::locateResources()
{
	//// load resource paths from config file
	//Ogre::ConfigFile cf;

	//Ogre::String resourcesPath = _fileSystemLayer->getConfigFilePath("resources.cfg");
	//if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	//{
	//	cf.load(resourcesPath);
	//}
	//else
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
	//		Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\media"),
	//		"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//}

	//Ogre::String sec, type, arch;
	//// go through all specified resource groups
	//Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	//for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
	//	sec = seci->first;
	//	const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
	//	Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

	//	// go through all resource paths
	//	for (i = settings.begin(); i != settings.end(); i++)
	//	{
	//		type = i->first;
	//		arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
	//		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
	//	}
	//}

	//sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	//const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	//OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

	//arch = genLocs.front().archive->getName();
	//type = genLocs.front().archive->getType();

	//// Add locations for supported shader languages
	//if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
	//}
	//else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

	//	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
	//	{
	//		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
	//	}
	//	else
	//	{
	//		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
	//	}

	//	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
	//	{
	//		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
	//	}
	//}
	//else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
	//}

	//mRTShaderLibPath = arch + "/RTShaderLib";
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/materials", type, sec);

	//if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSLES", type, sec);
	//}
	//else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
	//}
	//else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/HLSL", type, sec);
	//}
}

#include "OgreManager.h"

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreDataStream.h>
#include <OgreFileSystemLayer.h>



#include <SDL_video.h>
#include <SDL_syswm.h>

OgreManager::OgreManager(const Ogre::String& appName)
{
	_appName = appName;
	_fileSystemLayer = new Ogre::FileSystemLayer(appName);
	_root = nullptr;
	_overlaySystem = nullptr;
	_firstRun = true;

	//_shaderGenerator = nullptr;
	//_materialMgrListener = nullptr;
}

OgreManager::~OgreManager()
{
	close();
	delete _fileSystemLayer;
}

void OgreManager::init()
{
    // Crea el root de Ogre
    createRoot();

    // Inicia la ventana
	if (oneTimeConfig()) {
		setup();
	}

	// Agregado una prueba
	_sceneManager = _root->createSceneManager();
	
	// Create the camera
	Ogre::Camera* cam = _sceneManager->createCamera("Cam");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);
	//cam->setPolygonMode(Ogre::PM_WIREFRAME); 

	Ogre::SceneNode* camNode = _sceneManager->getRootSceneNode()->createChildSceneNode("nCam");
	camNode->attachObject(cam);
	camNode->setPosition(0, 0, 1000);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	//mCamNode->setDirection(Ogre::Vector3(0, 0, -1));

	// and tell it to render into the main window
	Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(1.0, 0.5, 0.0, 1.0));

	// Esto todavía no funciona porque falta el Resource Manager

	// Luz de prueba
	Ogre::Light* luz = _sceneManager->createLight("Luz");	//Cada luz tiene que tener un nombre diferente
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(1, 1, 1);

	Ogre::SceneNode* mLightNode = _sceneManager->getRootSceneNode()->createChildSceneNode("nLuz");
	//mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);
	mLightNode->setDirection(Ogre::Vector3(0.4, 0.2, -1));

	// Luz ambiente en la escena
	_sceneManager->setAmbientLight(Ogre::ColourValue(0.4f, 0.0f, 1.0f));
	

	std::cout << "OgreManager iniciado\n";
}

void OgreManager::update()
{
	//std::cout << "OgreManager actualizandose\n";

	// Renderiza el frame actual
	_root->renderOneFrame();
}

void OgreManager::close()
{
	if (_root != nullptr)
	{
		_root->saveConfig();
	}
	shutdown();
	delete _root;
	_root = nullptr;
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

void OgreManager::shutdown()
{
	// Destroy the RT Shader System.
	//destroyRTShaderSystem();

	// Borra la ventana
	if (_window.render != nullptr)
	{
		_root->destroyRenderTarget(_window.render);
		_window.render = nullptr;
	}

	// Borra el sistema Overlay
	delete _overlaySystem;
	_overlaySystem = nullptr;


	if (_window.native != nullptr)
	{
		SDL_DestroyWindow(_window.native);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		_window.native = nullptr;
	}
}

void OgreManager::setup()
{
	_root->initialise(false);
	createWindow(_appName);
	setWindowGrab(false);

	locateResources();
	//initialiseRTShaderSystem();
	loadResources();

	// adds context as listener to process context-level (above the sample level) events
	//_root->addFrameListener(this);
}

bool OgreManager::oneTimeConfig()
{
	if (!_root->restoreConfig())
	{
		return _root->showConfigDialog(NULL);
	}
	else return true;
}

//bool OgreManager::initialiseRTShaderSystem()
//{
//	if (Ogre::RTShader::ShaderGenerator::initialize())
//	{
//		mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
//		// Core shader libs not found -> shader generating will fail.
//		if (mRTShaderLibPath.empty())
//			return false;
//		// Create and register the material manager listener if it doesn't exist yet.
//		if (!mMaterialMgrListener) {
//			mMaterialMgrListener = new SGTechniqueResolverListener(mShaderGenerator);
//			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
//		}
//	}
//
//	return true;
//}
//
//void OgreManager::destroyRTShaderSystem()
//{
//	// Restore default scheme.
//	Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
//
//	// Unregister the material manager listener.
//	if (mMaterialMgrListener != nullptr)
//	{
//		Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
//		delete mMaterialMgrListener;
//		mMaterialMgrListener = nullptr;
//	}
//
//	// Destroy RTShader system.
//	if (mShaderGenerator != nullptr)
//	{
//		Ogre::RTShader::ShaderGenerator::destroy();
//		mShaderGenerator = nullptr;
//	}
//}

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

void OgreManager::setWindowGrab(bool _grab)
{
	SDL_bool grab = SDL_bool(_grab);
	SDL_SetWindowGrab(_window.native, grab);
	//SDL_SetRelativeMouseMode(grab);
	SDL_ShowCursor(grab);
}

//void OgreManager::pollEvents()
//{
//	if (_window.native == nullptr)
//		return;  // SDL events not initialized
//
//	SDL_Event event;
//	while (SDL_PollEvent(&event))
//	{
//		switch (event.type)
//		{
//		case SDL_QUIT:
//			_root->queueEndRendering();
//			break;
//		case SDL_WINDOWEVENT:
//			if (event.window.windowID == SDL_GetWindowID(_window.native)) {
//				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
//				{
//					Ogre::RenderWindow* win = mWindow.render;
//					//win->resize(event.window.data1, event.window.data2);  // IG2: ERROR 
//					win->windowMovedOrResized();
//					windowResized(win);
//				}
//			}
//			break;
//		default:
//			_fireInputEvent(convert(event));
//			break;
//		}
//	}
//
//	// just avoid "window not responding"
//	WindowEventUtilities::messagePump();
//}
//
//void IG2ApplicationContext::_fireInputEvent(const Event& event) const
//{
//	for (std::set<InputListener*>::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
//	{
//		InputListener& l = **it;
//
//		switch (event.type)
//		{
//		case KEYDOWN:
//			l.keyPressed(event.key);
//			break;
//		case KEYUP:
//			l.keyReleased(event.key);
//			break;
//		case MOUSEBUTTONDOWN:
//			l.mousePressed(event.button);
//			break;
//		case MOUSEBUTTONUP:
//			l.mouseReleased(event.button);
//			break;
//		case MOUSEWHEEL:
//			l.mouseWheelRolled(event.wheel);
//			break;
//		case MOUSEMOTION:
//			l.mouseMoved(event.motion);
//			break;
//		case FINGERDOWN:
//			// for finger down we have to move the pointer first
//			l.touchMoved(event.tfinger);
//			l.touchPressed(event.tfinger);
//			break;
//		case FINGERUP:
//			l.touchReleased(event.tfinger);
//			break;
//		case FINGERMOTION:
//			l.touchMoved(event.tfinger);
//			break;
//		}
//
//	}
//}

//bool OgreManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
//{
//	for (std::set<InputListener*>::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
//	{
//		(*it)->frameRendered(evt);
//	}
//
//	return true;
//}

void OgreManager::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreManager::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;

	Ogre::String resourcesPath = _fileSystemLayer->getConfigFilePath("resources.cfg");
	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{
		cf.load(resourcesPath);
	}
	else
	{
		// Lee la carpeta de modelos
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\models"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Lee la carpeta de materiales
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\materials"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Lee la carpeta de materiales
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(_solutionPath + "\\Assets\\textures"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	
	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	//OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();

	// Add locations for supported shader languages
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
		}
		else
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
		}

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
	}

	Ogre::String mRTShaderLibPath = arch + "/RTShaderLib";
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/materials", type, sec);
	
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/HLSL", type, sec);
	}
}

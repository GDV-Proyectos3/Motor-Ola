#pragma once

// NO ENCUENTRA UNA SERIE DE INCLUDES QUE DEBERÍA ENCONTRAR

#include <string>
#include "./utils/Singleton.h"
#include <Ogre.h>
#include "OverlayManager.h"
#include "OgreManager.h"


#include <OgreOverlayElement.h>

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>



#include "OgreOverlayPrerequisites.h"
#include <OgrePanelOverlayElement.h>
#include <OgreOverlay.h>
//#include <OgreRenderWindow.h>

#include <vector>
class Motor;
using CallBackOnClick = void(Motor* m);

class OverlayManager : public Singleton<OverlayManager> {
	
public:
	OverlayManager() {  };
	virtual ~OverlayManager();
	void init(OgreManager* om_,Motor* m);
	//virtual void init();
	//virtual void update() = 0;
	void update();

	void test() {
		//Singleton<OgreManager>().instance()->getSceneManager()->createOverlay();
	}
	void creaBoton(float x,float y,const std::string& texto, const std::string& nombrePanel, const std::string& nombreTexto,float tamLetra,const std::string& material,float dimX,float dimY/*, CallBackOnClick* click_*/);
	void setCallBackToButton(std::string p, CallBackOnClick* click_);
	void creaTexto(float x, float y, const std::string& texto, const std::string& nombreTexto, float tamLetra,const std::string& nombrePanel);
	void creaPanel(float x, float y,  const std::string& nombrePanel, const std::string& material, float dimX, float dimY);


private:
	//Ogre::Overlay* _overlay = nullptr;

	std::vector< Ogre::PanelOverlayElement*>botones;
	std::vector< Ogre::PanelOverlayElement*>paneles;
	std::vector< Ogre::PanelOverlayElement*>textos;
	std::map < Ogre::PanelOverlayElement*, CallBackOnClick*>callbacks;
	Ogre::Overlay* mOverlay=nullptr;
	//Usar Singleton cuando funcione
	OgreManager* og;
	Motor* motor;
	
};



#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

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

MOTOR_API class OverlayManager : public Singleton<OverlayManager> {
	
public:
	OverlayManager() {  };
	virtual ~OverlayManager();
	MOTOR_API void init(OgreManager* om_,Motor* m);
	MOTOR_API void update();

	MOTOR_API void creaBoton(float x,float y,const std::string& texto, const std::string& nombrePanel, const std::string& nombreTexto,float tamLetra,const std::string& material,float dimX,float dimY/*, CallBackOnClick* click_*/);
	MOTOR_API void setCallBackToButton(std::string p, CallBackOnClick* click_);
	MOTOR_API void creaTexto(float x, float y, const std::string& texto, const std::string& nombreTexto, float tamLetra,const std::string& nombrePanel);
	MOTOR_API void creaPanel(float x, float y,  const std::string& nombrePanel, const std::string& material, float dimX, float dimY);
	MOTOR_API Ogre::TextAreaOverlayElement* getTexto(std::string panelName, std::string textName);
	MOTOR_API Ogre::PanelOverlayElement* getPanel(std::string name);
	MOTOR_API Ogre::PanelOverlayElement* getBoton(std::string name);
	MOTOR_API void clear();
	MOTOR_API Motor* getMotor();


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



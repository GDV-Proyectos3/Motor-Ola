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


#include <vector>
class Motor;
class OgreManager;
using CallBackOnClick = void(Motor* m);

MOTOR_API class OverlayManager : public Singleton<OverlayManager> {
	
public:
	OverlayManager() {  };
	virtual ~OverlayManager();
	MOTOR_API void init(OgreManager* om_,Motor* m);
	MOTOR_API void update();

	MOTOR_API void creaBoton(float x,float y,const std::string& texto, const std::string& nombrePanel, const std::string& nombreTexto,float tamLetra,const std::string& material,float dimX,float dimY/*, CallBackOnClick* click_*/);
	MOTOR_API void setCallBackToButton(std::string p, CallBackOnClick* click_);
	MOTOR_API void creaTexto(float x, float y, const std::string& texto, const std::string& nombreTexto, float tamLetra,const std::string& nombrePanel,float dimX,float dimY);
	MOTOR_API void creaPanel(float x, float y,  const std::string& nombrePanel, const std::string& material, float dimX, float dimY);
	MOTOR_API Ogre::TextAreaOverlayElement* getTexto(std::string panelName, std::string textName);

	/// <summary>
	/// Solo cambia a rojo o negro
	/// </summary>
	/// <param name="panelName"></param>
	/// <param name="textName"></param>
	/// <param name="newColor">"Red" para rojo y ya</param>
	/// <returns></returns>
	MOTOR_API void changeTextColor(std::string panelName, std::string textName, std::string newColor);
	MOTOR_API Ogre::PanelOverlayElement* getPanel(std::string name);
	MOTOR_API Ogre::PanelOverlayElement* getBoton(std::string name);
	MOTOR_API void clear();
	MOTOR_API Motor* getMotor();


private:
	

	std::vector< Ogre::PanelOverlayElement*>botones;
	std::vector< Ogre::PanelOverlayElement*>paneles;
	std::vector< Ogre::PanelOverlayElement*>textos;
	std::map < Ogre::PanelOverlayElement*, CallBackOnClick*>callbacks;
	Ogre::Overlay* mOverlay=nullptr;
	
	OgreManager* og;
	Motor* motor;
	
};



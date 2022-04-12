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


class OverlayManager : public Singleton<OverlayManager> {
	
public:
	OverlayManager() {  };
	virtual ~OverlayManager() {
	};
	void init();
	//virtual void init();
	//virtual void update() = 0;

	void test() {
		//Singleton<OgreManager>().instance()->getSceneManager()->createOverlay();
	}

private:
	//Ogre::Overlay* _overlay = nullptr;
	
};



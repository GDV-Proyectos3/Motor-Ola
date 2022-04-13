#include "OverlayManager.h"
#include <string>
#include "InputManager.h"


void OverlayManager::init()
{

		
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	
		Ogre::OverlayContainer*container= static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Container"));
		container->setMetricsMode(Ogre::GMM_RELATIVE);
		container->setPosition(0.25, 0);
		container->setDimensions(0.5f, 1.0f);
		container->setColour(Ogre::ColourValue::Black);
		container->setMaterialName("Verde");

		
		
		paneles .push_back( static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", "PanelName")));
		paneles[0]->setMetricsMode(Ogre::GMM_RELATIVE);
		paneles[0]->setPosition(0.25/2, 0);
		paneles[0]->setDimensions(0.25f, 0.25f);
		paneles[0]->setMaterialName("Azul"); // Optional background material
		container->addChild(paneles[0]);
		

		// Ensures that the material exists
		//panel->setUV(0, 0, 0.5, 1);

		
		std::cout << paneles[0]->getZOrder()<<std::endl;
		// Create an overlay, and add the panel
		Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>(
			overlayManager.createOverlayElement("TextArea", "TextView"));
		
		
		text->setMetricsMode(Ogre::GMM_RELATIVE);
		text->setPosition(0.25/8, 0.25/4);
		
		text->setMaterialName("Rojo");
		text->setDimensions(0.1,0.1);
		text->setFontName("DejaVu/SerifCondensedItalic");
		text->setColour(Ogre::ColourValue::Black);
		text->setCaption("Hola que haces");
		text->setCharHeight(0.04f);
		paneles[0]->addChild(text);
		
		
		Ogre::Overlay* mOverlay = 0;
		
		mOverlay = overlayManager.create("PanelOverlay");
		mOverlay->setZOrder(500);
		
		//mOverlay->add2D(panel);
		mOverlay->add2D(container);
		mOverlay->show();
		
		
		
		/*Ogre::FrameListener* _framelistener = new Ogre::FrameListener();
		og().getRoot()->addFrameListener(_framelistener);*/
		
		
}
void OverlayManager::update() {
    
	if (ih().getMousePos().first > 239 && ih().getMousePos().first < 400 &&
		ih().getMousePos().second > 0 && ih().getMousePos().second < 120 
		) {
		paneles[0]->setMaterialName("Rojo");
		if(ih().getMouseButtonState(ih().LEFT)) {
			std::cout << "New Game" << std::endl;
		}
	}
	else {
		paneles[0]->setMaterialName("Azul");
	}
}



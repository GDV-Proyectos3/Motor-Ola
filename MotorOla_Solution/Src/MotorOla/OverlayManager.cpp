#include "OverlayManager.h"


void OverlayManager::init()
{
	   // man = new Ogre::OverlayManager();
		// load the fade
		//mFader = 0;
		//Ogre::MaterialPtr		mFaderMaterial;
		//Ogre::TextureUnitState* mTexture = 0;

		
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		Ogre::FontManager& fontManager = Ogre::FontManager::getSingleton();
		
		//Ogre::ResourceManager::
		//fontManager.create("BillboardText.fontdef", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	//	Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
		/**if (overlayManager == nullptr) {
			std::cout << "overlaymanager is null" << std::endl;
			
		}*/

		//mFader = static_cast<Ogre::PanelOverlayElement*>(
		//	overlayManager->createOverlayElement("Panel", "Fader"));
		//mFader = static_cast<Ogre::PanelOverlayElement*>(
			//	man->createOverlayElement("Panel.overlay", "Fader"));
		Ogre::OverlayContainer*container= static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Container"));
		container->setMetricsMode(Ogre::GMM_RELATIVE);
		container->setPosition(0, 0);
		container->setDimensions(0.5f, 0.5f);
		container->setCaption("Container");
		container->setMaterialName("Verde");

		container->setColour(Ogre::ColourValue::Black);
		
		Ogre::PanelOverlayElement* panel = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", "PanelName"));
		panel->setMetricsMode(Ogre::GMM_RELATIVE);
		panel->setPosition(0, 0);
		panel->setDimensions(0.25f, 0.25f);
		panel->setMaterialName("Azul"); // Optional background material
		container->addChild(panel);

		// Ensures that the material exists
		//panel->setUV(0, 0, 0.5, 1);

		// show the fade
		panel->show();
		std::cout << panel->getZOrder()<<std::endl;
		// Create an overlay, and add the panel
		Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>(
			overlayManager.createOverlayElement("TextArea", "TextView"));
		//text->setFontName("Arial");
		text->setMetricsMode(Ogre::GMM_RELATIVE);
		text->setColour(Ogre::ColourValue::Black);
		text->setDimensions(1.0f, 1.0f);
		//text->setFontName()
		text->setCaption("Hola");
		text->setCharHeight(0.03f);
		container->addChild(text);
		//panel->addChild(text);
		//text->show();
		std::cout << text->getZOrder() << std::endl;
		container->show();
		
		Ogre::Overlay* mOverlay = 0;
		//mOverlay = overlayManager->create("FaderOverlay");
		mOverlay = overlayManager.create("PanelOverlay");
		
		//mOverlay->add2D(panel);
		mOverlay->add2D(container);
		mOverlay->show();
		/*Ogre::FrameListener* _framelistener = new Ogre::FrameListener();
		og().getRoot()->addFrameListener(_framelistener);*/
		
		
}



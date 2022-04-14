#include "OverlayManager.h"
#include <string>
#include "InputManager.h"
#include "Motor.h"


void OverlayManager::init(OgreManager*om,Motor* m)
{

		
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		mOverlay = 0;
		
		mOverlay = overlayManager.create("PanelOverlay");
		mOverlay->setZOrder(100);
		og = om;//Usar Singleton cuando funcione
		motor = m;
		
		
		
}
void OverlayManager::update() {
    
	auto it=botones.begin();
	bool dado = false;
	
	
	while (dado == false && it != botones.end()) {
		if (ih().getMousePos().first*1.0f / og->getRenderWindow()->getWidth() >(*it)->getLeft() && ih().getMousePos().first * 1.0f / og->getRenderWindow()->getWidth() <(*it)->getLeft() + (*it)->getWidth() &&
			ih().getMousePos().second*1.0f / og->getRenderWindow()->getHeight() >(*it)->getTop() && ih().getMousePos().second*1.0f / og->getRenderWindow()->getHeight() < (*it)->getTop() + (*it)->getHeight())
		{
			(*it)->setMaterialName("Rojo");
			if (ih().getMouseButtonState(ih().LEFT)) {
				callbacks.at((*it))(motor);
				dado = true;
			}
		}
		else {
			(*it)->setMaterialName("Azul");
		}
		if (dado == false) {
			it++;
		}
		
	}

}

void OverlayManager::creaBoton(float x, float y, const std::string& textoBoton,const std::string& nombrePanel,const std::string& nombreTexto,float tamLetra, const std::string& material, float dimX, float dimY,CallBackOnClick* click_)
{
	if (tamLetra > 0.1) {
		tamLetra = 0.1;//Si no se sale del panel con el tamaño que tiene
	}
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	botones.push_back(static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", nombrePanel)));
	botones.back()->setMetricsMode(Ogre::GMM_RELATIVE);
	botones.back()->setPosition(x, y);
	botones.back()->setDimensions(dimX,dimY);
	botones.back()->setMaterialName(material); // Optional background material
	
	//container->addChild(paneles[0]);


	// Ensures that the material exists
	//panel->setUV(0, 0, 0.5, 1);


	
	// Create an overlay, and add the panel
	Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", nombreTexto));
	

	text->setMetricsMode(Ogre::GMM_RELATIVE);
	text->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
	text->setPosition((botones.back()->getWidth() / 2) , ((botones.back()->getHeight()) / 2)-(tamLetra/2));
	text->setMaterialName("Rojo");
	
	text->setFontName("DejaVu/SerifCondensedItalic");
	text->setColour(Ogre::ColourValue::Black);
	text->setCaption(textoBoton);
	text->setCharHeight(tamLetra);
	botones.back()->addChild(text);
	mOverlay->add2D(botones.back());
	mOverlay->show();
	callbacks.insert(std::pair(botones.back(), click_));
}

void OverlayManager::creaTexto(float x, float y, const std::string& texto, const std::string& nombreTexto, float tamLetra,const std::string& nombrePanel)
{
	
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	textos.push_back(static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", nombrePanel)));
	textos.back()->setMetricsMode(Ogre::GMM_RELATIVE);
	textos.back()->setPosition(x, y);
	textos.back()->setDimensions(0.25, 0.25);
	
	Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", nombreTexto));


	text->setMetricsMode(Ogre::GMM_RELATIVE);
	text->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
	text->setPosition((textos.back()->getWidth() / 2), ((textos.back()->getHeight()) / 2) - (tamLetra / 2));
	//text->setMaterialName("Rojo");

	text->setFontName("DejaVu/SerifCondensedItalic");
	text->setColour(Ogre::ColourValue::Black);
	text->setCaption(texto);
	text->setCharHeight(tamLetra);
	textos.back()->addChild(text);
	mOverlay->add2D(textos.back());
	mOverlay->show();
	
	
}

void OverlayManager::creaPanel(float x, float y, const std::string& nombrePanel, const std::string& material, float dimX, float dimY)
{
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	paneles.push_back(static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", nombrePanel)));
	paneles.back()->setMetricsMode(Ogre::GMM_RELATIVE);
	paneles.back()->setPosition(x, y);
	paneles.back()->setDimensions(dimX, dimY);
	paneles.back()->setMaterialName(material); // Optional background material
	mOverlay->add2D(paneles.back());
}



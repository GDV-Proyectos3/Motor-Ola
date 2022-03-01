#include "prueba.h"
prueba::prueba()
{
}
prueba::~prueba()
{
}
void prueba::init() {
	//std::cout << "Motor Ogre";
    std::cout << "Hello Ogre!\n";
    Ogre::Root* root;
    root = new Ogre::Root();
    system("Pause");
}
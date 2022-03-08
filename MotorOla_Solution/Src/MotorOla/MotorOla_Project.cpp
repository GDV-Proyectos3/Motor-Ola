#include <iostream>
#include "Motor.h"

int main()
{
    Motor* m = new Motor();
    m->initSystems();
    m->updateSystems();
    delete m;
    return 0;
}


// Hola mundo! OGRE (prueba básica)
/*#include <OgreRoot.h>
int main()
{
    Ogre::Root* root;
    root = new Ogre::Root();
    return 0;
}*/

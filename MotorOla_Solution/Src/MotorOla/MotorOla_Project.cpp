#include <iostream>
//#include <OgreRoot.h>
#include "Motor.h"

int main()
{
    Motor* m = new Motor();
    m->initSystems();
    m->updateSystems();
    return 0;
   /* std::cout << "Hello Ogre!\n";
    Ogre::Root* root;
    root = new Ogre::Root();
    system("Pause");
    return 0;*/
}

/*#include <iostream>
#include "Motor.h"

int main()
{
    // Inicia el Motor
    Motor* m = new Motor();
    bool hayError = false;
    
    // Intenta iniciar los sistemas principales
    try {
        m->initSystems();
    }
    catch (const char* error) {
        std::cout << "Error iniciando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
    }

    // Pone en marcha el bucle principal del Motor
    try {
        m->updateSystems();
    }
    catch (const char* error) {
        std::cout << "Error durante el bucle principal del motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
    }
    
    // Libera la memoria del Motor y los sistemas principales
    delete m;

    return hayError;
}*/


// Hola mundo! OGRE (prueba básica)
/*#include <OgreRoot.h>
int main()
{
    Ogre::Root* root;
    root = new Ogre::Root();
    return 0;
}*/

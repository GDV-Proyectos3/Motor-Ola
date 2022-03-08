#include <iostream>
//#include <OgreRoot.h>
#include "Motor.h"

int main()
{
    Motor* m = new Motor();
    m->initSystems();
    m->updateSystems();
    delete m;
    return 0;
}

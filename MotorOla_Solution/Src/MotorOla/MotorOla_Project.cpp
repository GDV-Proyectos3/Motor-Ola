#include <iostream>
//#include <OgreRoot.h>
#include "Motor.h"

int main()
{
    Motor* m = new Motor();
    m->initSystems();
    m->updateSystems();
    return 0;
}

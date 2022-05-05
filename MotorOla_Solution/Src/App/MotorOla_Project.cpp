#include <iostream>
#include <Motor.h>

#if _DEBUG
    #include "utils/checkML.h"
#endif

int main()
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::cout << "APLICACION INICIADA\n";

    bool hayError = false;      
    Motor* m = nullptr;
    
    // Inicia el Motor
    try {
        std::cout << "NEW MOTOR\n";
        m = new Motor();
    }
    catch (const char* error) {
        std::cout << "Error creando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
    }
    
    // Intenta iniciar los sistemas principales
    try {
        std::cout << "MOTOR INIT SISTEMS\n";
        m->initSystems();
    }
    catch (const char* error) {
        std::cout << "Error iniciando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
        system("pause");
    }

    // Pone en marcha el bucle principal del Motor
    try {
        std::cout << "MOTOR START MAIN LOOP\n";
        m->mainLoop();
    }
    catch (const char* error) {
        std::cout << "Error durante el bucle principal del motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
        system("pause");
    }
    
    // Libera la memoria del Motor y los sistemas principales
    if (m != nullptr) {
        delete m;
        m = nullptr;
    }

    return hayError;
}
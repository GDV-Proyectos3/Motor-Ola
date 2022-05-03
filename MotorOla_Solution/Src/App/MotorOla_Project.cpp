#include <iostream>
#include <Motor.h>

int main()
{
    std::cout << "APLICACION INICIADA\n";
    
    bool hayError = false;      
    Motor* m = nullptr;
    
    // Inicia el Motor
    try {
        std::cout << "PRIMER TRY\n";
        m = new Motor();
    }
    catch (const char* error) {
        std::cout << "Error creando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
    }
    
    // Intenta iniciar los sistemas principales
    try {
        std::cout << "SEGUNDO TRY\n";
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

    system("pause");

    return hayError;
}
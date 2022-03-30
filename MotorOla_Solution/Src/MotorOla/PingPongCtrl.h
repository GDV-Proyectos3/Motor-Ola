#pragma once
#include "Componente.h"
#include <SDL.h>
#include "InputManager.h"
#include <iostream>
//#include <SDL_scancode.h>
class Transform;
class PingPongCtrl :
    public Componente
{
public:
    PingPongCtrl(SDL_Scancode up, SDL_Scancode down);
    ~PingPongCtrl();
    void update();
private:
    SDL_Scancode u;
    SDL_Scancode d;
    
};


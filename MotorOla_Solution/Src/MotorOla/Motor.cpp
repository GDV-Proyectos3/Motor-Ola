#include "Motor.h"
//#include "prueba.h"
Motor::Motor()
{
	//Crea los diferentes singletons
	//p = new prueba();
	
	
	
	
}

Motor::~Motor()
{
	//Destruye los singletons
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

void Motor::initSystems()
{
	//Inicializa a cada uno de los singletons
	//p->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Prueba SDL", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINWIDTH, WINHEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
void Motor::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !stop) {
		if (event.type==SDL_QUIT||event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) {
			stop = true;
		}
		else {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
				std::cout << "Bucle" << std::endl;
			}
		}

	}
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		handleEvents();
		//Input
		//Update
		//Render
		
		
	}
}

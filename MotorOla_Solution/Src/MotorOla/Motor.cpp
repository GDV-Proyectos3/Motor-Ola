#include "Motor.h"
#include "prueba.h"
Motor::Motor()
{
	//Crea los diferentes singletons
	p = new prueba();
	
	
	
	
}

Motor::~Motor()
{
	//Destruye los singletons
	delete p;
	p = nullptr;
}

void Motor::initSystems()
{
	//Inicializa a cada uno de los singletons
	p->init();
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		//Input
		//Update
		//Render
		p->update();
		
	}
}

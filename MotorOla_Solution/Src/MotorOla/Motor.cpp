#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "utils/Singleton.h"

class Transform : public Componente {
public:
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual ~Transform() {}
};
class Rigidbody : public Componente {
public:
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual ~Rigidbody() {}
};

Motor::Motor()
{
	// Inicia los managers
	if(!_ogreManager) _ogreManager = new OgreManager();	
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
}

void Motor::initSystems()
{
	_ogreManager->init();
	

	// Registrando Componentes
	ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	ComponenteRegistro::ComponenteRegistro<Rigidbody>("rigidbody");
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		_ogreManager->update();	// Renderiza un frame
		//handleEvents();
		//Input
		//Update
		//Render
		
		
	}
}

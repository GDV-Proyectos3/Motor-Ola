#include "PingPongCtrl.h"
#include "Transform.h"
#include "Entidad.h"

PingPongCtrl::PingPongCtrl(SDL_Scancode up, SDL_Scancode down)
{
	u = up;
	d = down;
	
}

PingPongCtrl::~PingPongCtrl()
{
}

void PingPongCtrl::update()
{
	//pala2->getComponent<Transform>()->setPosY(pala2->getComponent<Transform>()->getPos().getY() + 10.0f);
	if (ih().isKeyDown(u)) {
		entity_->getComponent<Transform>()->setPosY(entity_->getComponent<Transform>()->getPos().getY() + 10.0f);
		//std::cout << "Tecla: " << u << std::endl;
	}
	else if (ih().isKeyDown(d)) {
		entity_->getComponent<Transform>()->setPosY(entity_->getComponent<Transform>()->getPos().getY() -10.0f);
		//std::cout << "Tecla: " << d << std::endl;
	}
}

#include "Corridor.h"

Corridor::Corridor() : GameObject(GameObject::Type::Corridor)
{
	walkable_ = true;
	color_ = GameObject::Color::Green_Black;
}

Corridor::~Corridor()
{
}
#include "Corridor.h"

Corridor::Corridor() : GameObject(GameObject::Type::Corridor)
{
	walkable_ = true;
}

Corridor::~Corridor()
{
}
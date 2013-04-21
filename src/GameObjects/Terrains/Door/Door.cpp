#include "Door.h"

Door::Door() : GameObject(GameObject::Type::Door)
{
	walkable_ = false;
}

Door::~Door()
{
}
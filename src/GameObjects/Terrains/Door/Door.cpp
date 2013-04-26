#include "Door.h"

Door::Door() : GameObject(GameObject::Type::Door)
{
	walkable_ = false;
}

Door::~Door()
{
}

void Door::open()
{
	in_fov_ = true;
	walkable_ = true;
	type_ = GameObject::Type::OpenedDoor;
}
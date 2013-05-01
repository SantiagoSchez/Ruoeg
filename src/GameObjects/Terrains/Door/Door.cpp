#include "Door.h"

Door::Door() : GameObject(GameObject::Type::Door)
{
	walkable_ = false;
	color_ = GameObject::Color::Cyan_Black;
}

Door::~Door()
{
}

void Door::open()
{
	discovered_ = true;
	walkable_ = true;
	type_ = GameObject::Type::OpenedDoor;
	color_ = GameObject::Color::Green_Black;
}
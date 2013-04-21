#include "Dragon.h"

Dragon::Dragon() : GameObject(GameObject::Type::Dragon)
{
	walkable_ = false;
}

Dragon::~Dragon()
{
}
#include "Chest.h"

Chest::Chest() : GameObject(GameObject::Type::Chest)
{
	walkable_ = false;
}

Chest::~Chest()
{
}
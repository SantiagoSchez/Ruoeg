#include "Goblin.h"

Goblin::Goblin() : GameObject(GameObject::Type::Goblin)
{
	walkable_ = false;
}

Goblin::~Goblin()
{
}
#include "Stairs.h"

Stairs::Stairs() : GameObject(GameObject::Type::DownStairs)
{
	walkable_ = true;
}

Stairs::~Stairs()
{
}
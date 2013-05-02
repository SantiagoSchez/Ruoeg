#include "Stairs.h"

Stairs::Stairs() : GameObject(GameObject::Type::DownStairs)
{
	walkable_ = false;
}

Stairs::~Stairs()
{
}
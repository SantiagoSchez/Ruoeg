#include "SmallGoblin.h"

SmallGoblin::SmallGoblin() : GameObject(GameObject::Type::SmallGoblin)
{
	walkable_ = false;
}

SmallGoblin::~SmallGoblin()
{
}
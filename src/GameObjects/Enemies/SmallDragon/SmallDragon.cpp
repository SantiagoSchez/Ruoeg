#include "SmallDragon.h"

SmallDragon::SmallDragon() : GameObject(GameObject::Type::SmallDragon)
{
	walkable_ = false;
}

SmallDragon::~SmallDragon()
{
}
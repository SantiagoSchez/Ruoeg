#include "Skeleton.h"

Skeleton::Skeleton() : GameObject(GameObject::Type::Skeleton)
{
	walkable_ = false;
}

Skeleton::~Skeleton()
{
}
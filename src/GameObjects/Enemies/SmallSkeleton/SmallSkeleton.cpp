#include "SmallSkeleton.h"

SmallSkeleton::SmallSkeleton() : GameObject(GameObject::Type::SmallSkeleton)
{
	walkable_ = false;
}

SmallSkeleton::~SmallSkeleton()
{
}
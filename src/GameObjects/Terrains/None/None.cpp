#include "None.h"

None::None() : GameObject(GameObject::Type::None)
{
	walkable_ = false;
}

None::~None()
{
}
#include "None.h"

None::None() : GameObject(GameObject::Type::None)
{
	walkable_ = false;
	color_ = GameObject::Color::Black_Black;
}

None::~None()
{
}
#include "Lit.h"

Lit::Lit() : GameObject(GameObject::Type::Lit)
{
	walkable_ = true;
	color_ = GameObject::Color::Green_Black;
}

Lit::~Lit()
{
}
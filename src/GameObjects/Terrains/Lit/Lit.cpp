#include "Lit.h"

Lit::Lit() : GameObject(GameObject::Type::Lit)
{
	walkable_ = true;
}

Lit::~Lit()
{
}
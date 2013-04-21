#include "VerticalWall.h"

VerticalWall::VerticalWall() : GameObject(GameObject::Type::VerticalWall)
{
	walkable_ = false;
}

VerticalWall::~VerticalWall()
{
}
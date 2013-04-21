#include "HorizontalWall.h"

HorizontalWall::HorizontalWall() : GameObject(GameObject::Type::HorizontalWall)
{
	walkable_ = false;
}

HorizontalWall::~HorizontalWall()
{
}
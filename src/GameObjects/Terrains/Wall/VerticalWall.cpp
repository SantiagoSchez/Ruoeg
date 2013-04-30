#include "VerticalWall.h"

VerticalWall::VerticalWall() : GameObject(GameObject::Type::VerticalWall)
{
	walkable_ = false;
	color_ = GameObject::Color::Red_Black;
}

VerticalWall::~VerticalWall()
{
}
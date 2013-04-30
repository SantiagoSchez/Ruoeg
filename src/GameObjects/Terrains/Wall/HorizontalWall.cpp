#include "HorizontalWall.h"

HorizontalWall::HorizontalWall() : GameObject(GameObject::Type::HorizontalWall)
{
	walkable_ = false;
	color_ = GameObject::Color::Red_Black;
}

HorizontalWall::~HorizontalWall()
{
}
#include "GameObject.h"

GameObject::GameObject(Type type) : type_(type), in_fov_(false)
{
}

GameObject::~GameObject()
{
}

GameObject::Type GameObject::type() const
{
	return type_;
}

bool GameObject::walkable() const
{
	return walkable_;
}

bool GameObject::in_fov() const
{
	return in_fov_;
}

void GameObject::set_in_fov(bool in_fov)
{
	in_fov_ = in_fov;
}

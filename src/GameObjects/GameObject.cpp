#include "GameObject.h"

GameObject::GameObject(Type type) : type_(type), enemy_(false),
	in_fov_(false), delete_object_(false), color_(GameObject::Color::None)
{
}

GameObject::~GameObject()
{
}

GameObject::Color GameObject::color() const
{
	return color_;
}

GameObject::Type GameObject::type() const
{
	return type_;
}

bool GameObject::enemy() const
{
	return enemy_;
}

bool GameObject::walkable() const
{
	return walkable_;
}

bool GameObject::delete_object() const
{
	return delete_object_;
}

void GameObject::kill_object()
{
	delete_object_ = true;
}

bool GameObject::in_fov() const
{
	return in_fov_;
}

void GameObject::set_in_fov(bool in_fov)
{
	in_fov_ = in_fov;
}
#include "GameObject.h"

GameObject::GameObject(Type type) : type_(type), discovered_(false), 
	delete_object_(false), color_(GameObject::Color::None)
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

bool GameObject::discovered() const
{
	return discovered_;
}

void GameObject::set_discovered(bool discovered)
{
	discovered_ = discovered;
}
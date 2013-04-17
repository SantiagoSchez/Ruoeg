#include "GameObject.h"

GameObject::GameObject(Type type) : type_(type) 
{
}

GameObject::~GameObject()
{
}

GameObject::Type GameObject::type() const
{
	return type_;
}
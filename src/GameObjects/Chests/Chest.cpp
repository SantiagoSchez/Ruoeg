#include "Chest.h"

int Chest::num_chests_;

Chest::Chest() : GameObject(GameObject::Type::Chest)
{
	walkable_ = false;
	++num_chests_;
}

Chest::~Chest()
{
	--num_chests_;
}

int Chest::num_chests()
{
	return num_chests_;
}
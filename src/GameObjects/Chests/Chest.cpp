#include "Chest.h"

int Chest::num_chests_;

Chest::Chest() : GameObject(GameObject::Type::Chest)
{
	walkable_ = false;
	++num_chests_;
}

Chest::~Chest()
{
}

void Chest::reset_num_chests()
{
	num_chests_ = 0;
}

int Chest::num_chests()
{
	return num_chests_;
}

void Chest::decrease_num_chests()
{
	--num_chests_;
}

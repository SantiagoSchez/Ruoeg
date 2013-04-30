#include "Chest.h"

int Chest::num_chests_;

Chest::Chest(int score) : GameObject(GameObject::Type::Chest), score_(score)
{
	walkable_ = false;
	++num_chests_;
	color_ = GameObject::Color::Cyan_Black;
}

Chest::~Chest()
{
	--num_chests_;
}

int Chest::num_chests()
{
	return num_chests_;
}

int Chest::score() const
{
	return score_;
}

#include "Troll.h"

Troll::Troll() : Enemy(GameObject::Type::Troll)
{
	walkable_ = false;
	name_ = "Big Troll";
	color_ = GameObject::Color::Yellow_Red;

	level_ = 2;
	health_ = max_health_ = 50;
	attack_ = 10;
	armor_ = 6;
}

Troll::~Troll()
{
}

void Troll::levelUp()
{
	++level_;
	health_ += 50;
	attack_ += 5;
	armor_ += 5;
}
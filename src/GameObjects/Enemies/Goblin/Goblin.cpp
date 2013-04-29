#include "Goblin.h"

Goblin::Goblin() : Enemy(GameObject::Type::Goblin)
{
	walkable_ = false;
	name_ = "Big Goblin";

	level_ = 2;
	health_ = max_health_ = 50;
	attack_ = 10;
	armor_ = 6;
}

Goblin::~Goblin()
{
}

void Goblin::levelUp()
{
	++level_;
	health_ += 50;
	attack_ += 5;
	armor_ += 5;
}
#include "SmallDragon.h"

SmallDragon::SmallDragon() : Enemy(GameObject::Type::SmallDragon)
{
	walkable_ = false;
	name_ = "Small Dragon";

	level_ = 1;
	health_ = max_health_ = 25;
	attack_ = 5;
	armor_ = 3;
}

SmallDragon::~SmallDragon()
{
}

void SmallDragon::levelUp()
{
	++level_;
	health_ += 10;
	attack_ += 2;
	armor_ += 2;
}
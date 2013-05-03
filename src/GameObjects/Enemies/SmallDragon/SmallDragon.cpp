#include "SmallDragon.h"

SmallDragon::SmallDragon(Dungeon &dungeon, int x, int y, int score, int exp) : 
	Enemy(GameObject::Type::SmallDragon, dungeon, x, y, score, exp)
{
	walkable_ = false;
	name_ = "Small Dragon";
	color_ = GameObject::Color::Yellow_Black;

	level_ = 0;
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
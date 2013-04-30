#include "SmallTroll.h"

SmallTroll::SmallTroll() : Enemy(GameObject::Type::SmallTroll)
{
	walkable_ = false;
	name_ = "Troll";
	color_ = GameObject::Color::Yellow_Black;

	level_ = 1;
	health_ = max_health_ = 25;
	attack_ = 5;
	armor_ = 3;
}

SmallTroll::~SmallTroll()
{
}

void SmallTroll::levelUp()
{
	++level_;
	health_ += 10;
	attack_ += 2;
	armor_ += 2;
}
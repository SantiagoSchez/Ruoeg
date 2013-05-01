#include "Dragon.h"

Dragon::Dragon(Dungeon &dungeon, int x, int y) : 
	Enemy(GameObject::Type::Dragon, dungeon, x, y)
{
	walkable_ = false;
	name_ = "Big Dragon";
	color_ = GameObject::Color::Yellow_Red;

	level_ = 2;
	health_ = max_health_ = 50;
	attack_ = 10;
	armor_ = 6;
}

Dragon::~Dragon()
{
}

void Dragon::levelUp()
{
	++level_;
	health_ += 50;
	attack_ += 5;
	armor_ += 5;
}
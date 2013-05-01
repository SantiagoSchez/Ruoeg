#include "Goblin.h"

Goblin::Goblin(Dungeon &dungeon, int x, int y) : 
	Enemy(GameObject::Type::Goblin, dungeon, x, y)
{
	walkable_ = false;
	name_ = "Big Goblin";
	color_ = GameObject::Color::Yellow_Red;

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
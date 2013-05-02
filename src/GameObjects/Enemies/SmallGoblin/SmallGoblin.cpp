#include "SmallGoblin.h"

SmallGoblin::SmallGoblin(Dungeon &dungeon, int x, int y, int score, int exp) : 
	Enemy(GameObject::Type::SmallGoblin, dungeon, x, y, score, exp)
{
	walkable_ = false;
	name_ = "Goblin";
	color_ = GameObject::Color::Yellow_Black;

	level_ = 1;
	health_ = max_health_ = 25;
	attack_ = 5;
	armor_ = 3;
}

SmallGoblin::~SmallGoblin()
{
}

void SmallGoblin::levelUp()
{
	++level_;
	health_ += 10;
	attack_ += 2;
	armor_ += 2;
}
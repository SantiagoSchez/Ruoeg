#include "Skeleton.h"

Skeleton::Skeleton(Dungeon &dungeon, int x, int y, int score, int exp) : 
	Enemy(GameObject::Type::Skeleton, dungeon, x, y, score, exp)
{
	walkable_ = false;
	name_ = "Big Skeleton";
	color_ = GameObject::Color::Yellow_Red;

	level_ = 1;
	health_ = max_health_ = 50;
	attack_ = 10;
	armor_ = 6;
}

Skeleton::~Skeleton()
{
}

void Skeleton::levelUp()
{
	++level_;
	health_ += 50;
	attack_ += 5;
	armor_ += 5;
}
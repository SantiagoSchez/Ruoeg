#include "SmallSkeleton.h"

SmallSkeleton::SmallSkeleton() : Enemy(GameObject::Type::SmallSkeleton)
{
	walkable_ = false;
	name_ = "Skeleton";
	color_ = GameObject::Color::Yellow_Black;

	level_ = 1;
	health_ = max_health_ = 25;
	attack_ = 5;
	armor_ = 3;
}

SmallSkeleton::~SmallSkeleton()
{
}

void SmallSkeleton::levelUp()
{
	++level_;
	health_ += 10;
	attack_ += 2;
	armor_ += 2;
}
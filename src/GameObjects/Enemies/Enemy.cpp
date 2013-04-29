#include "Enemy.h"

Enemy::Enemy(Type type) : GameObject(type), name_("!"), level_(-1),
	health_(-1), max_health_(-1), attack_(-1), armor_(-1)
{
	walkable_ = false;
}

Enemy::~Enemy()
{
}

const char* Enemy::name() const
{
	return name_;
}

int Enemy::level() const
{
	return level_;
}

int Enemy::health() const
{
	return health_;
}

int Enemy::max_health() const
{
	return max_health_;
}

int Enemy::attack() const
{
	return attack_;
}

int Enemy::armor() const
{
	return armor_;
}

int Enemy::receiveDamage(int attack_points)
{
	int damage = attack_points - armor_;
	health_ -= damage;

	return damage;
}
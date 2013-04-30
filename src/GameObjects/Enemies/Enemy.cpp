#include "Enemy.h"

int Enemy::num_enemies_;

Enemy::Enemy(Type type) : GameObject(type), name_("!"), level_(-1),
	health_(-1), max_health_(-1), attack_(-1), armor_(-1)
{
	walkable_ = false;
	++num_enemies_;
	enemy_ = true;
}

Enemy::~Enemy()
{
	--num_enemies_;
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
	int damage = rng_.nextInt(attack_points-2, attack_points+2) - armor_;
	if(damage <= 0)
	{
		damage = 1;
	}
	health_ -= damage;

	if(health_ <= 0)
	{
		delete_object_ = true;
		health_ = 0;
	}

	return damage;
}

const char* Enemy::toString()
{
	string_ = std::to_string(level_) + " | [" +
		std::to_string(health_) + "/" + std::to_string(max_health_) +
		"] | " + name_;

	return string_.c_str();
}

int Enemy::num_enemies()
{
	return num_enemies_;
}

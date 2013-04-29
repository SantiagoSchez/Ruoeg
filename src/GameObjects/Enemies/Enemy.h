//
// A class for holding enemies
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_

#include "../GameObject.h"
#include "../../Utils/RNG.h"
#include <string>

class Enemy : public GameObject
{
public:
	Enemy(Type type);
	virtual ~Enemy();
	
	virtual const char* name() const;
	virtual int level() const;
	virtual int health() const;
	virtual int max_health() const;
	virtual int attack() const;
	virtual int armor() const;
	virtual int receiveDamage(int attack_points);
	virtual const char* toString();

	virtual void levelUp() = 0;

	static int num_enemies();

protected:
	char *name_;
	int level_;
	int health_;
	int max_health_;
	int attack_;
	int armor_;

	RNG rng_;
	std::string string_;

	static int num_enemies_;
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_
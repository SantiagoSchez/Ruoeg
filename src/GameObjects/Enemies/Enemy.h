//
// A class for holding enemies
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_

#include "../../Utils/RNG.h"
#include "../../Utils/Point.h"
#include "../../Curses/Curses.h"
#include "../GameObject.h"
#include "../../Map/Map2D.h"

#include <string>

class Player;
class Dungeon;

class Enemy : public GameObject
{
public:
	Enemy(Type type, Dungeon &dungeon, int x, int y, int score, int exp);
	virtual ~Enemy();
	
	virtual const char* name() const;
	virtual int level() const;
	virtual int health() const;
	virtual int max_health() const;
	virtual int attack() const;
	virtual int armor() const;
	virtual int receiveDamage(Player &player);
	virtual const char* toString();
	virtual Point& location();

	virtual void draw(WINDOW *win);
	virtual void update(Player &player);

	virtual void levelUp() = 0;

	static int num_enemies();

protected:
	virtual bool moveNorth(Player &player);
	virtual bool moveEast(Player &player);
	virtual bool moveSouth(Player &player);
	virtual bool moveWest(Player &player);

	Point location_;
	Dungeon &dungeon_;

	char *name_;
	int level_;
	int health_;
	int max_health_;
	int attack_;
	int armor_;
	int score_;
	int exp_;

	RNG rng_;
	std::string string_;

	static int num_enemies_;
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_ENEMY_H_
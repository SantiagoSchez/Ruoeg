//
// An abstract class representing the player in the game
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_H_
#define RUOEG_GAMEOBJECTS_PLAYER_H_

#include "../../Utils/Point.h"
#include "../../Utils/Direction.h"
#include "../../Map/Dungeon.h"
#include "../Chests/Chest.h"
#include "../Terrains/Door/Door.h"

class Enemy;

class Player : public GameObject
{
public:
	enum class Race
	{
		Human = 1,	// =Health=Attack=Armor
		Elf = 2,	// -Health-Attack-Armor
		Orc = 3,	// +Health+Attack+Armor
		Dwarf = 4	// -Health+Attack+Armor
	};

	Player(Race race, Dungeon &dungeon);
	virtual ~Player();

	virtual Race race() const;
	virtual const char* str_race() const;
	virtual int health_points() const;
	virtual int max_health_points() const;
	virtual int attack_points() const;
	virtual int armor_points() const;
	virtual int experience_points() const;
	virtual int max_experience_points() const;
	virtual void addExp(int exp);
	virtual int level() const;
	virtual bool isAlive() const;
	virtual int receiveDamage(Enemy &enemy);
	virtual bool goDeep();

	virtual int explored() const;
	virtual void reset_explored();

	virtual const Point& location() const;
	virtual void placeIt(int x, int y);

	virtual bool moveNorth();
	virtual bool moveEast();
	virtual bool moveSouth();
	virtual bool moveWest();

	virtual void openDoor(Door &door);
	virtual void openChest(Chest &chest);
	virtual void attack(Enemy &enemy);

	virtual void levelUp() = 0;
	virtual void draw(WINDOW *win) = 0;

	// DEBUG
	bool ghost_mode;
protected:
	virtual void checkMapCollisions(Tile &tile);
	virtual void doFOV();

	RNG rng_;
	Race race_;
	char *str_race_;
	Dungeon &dungeon_;

	int health_points_;
	int attack_points_;
	int max_health_points_;
	int armor_points_;
	int experience_points_;
	int max_experience_points_;
	int level_;
	int explored_;
	Point location_;
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_H_
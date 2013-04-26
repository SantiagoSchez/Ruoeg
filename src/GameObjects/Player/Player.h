//
// An abstract class representing the player in the game
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_H_
#define RUOEG_GAMEOBJECTS_PLAYER_H_

#include "../GameObject.h"
#include "../../Map/Dungeon.h"

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

	explicit Player(Race race, Map2D &map);
	virtual ~Player();

	virtual Race race() const;
	virtual const char* str_race() const;
	virtual int health_points() const;
	virtual int attack_points() const;
	virtual int armor_points() const;
	virtual int experience_points() const;
	virtual int max_experience_points() const;
	virtual int level() const;
	virtual bool isAlive() const;

	virtual int explored() const;
	virtual void reset_explored();

	virtual const Dungeon::Point& location() const;
	virtual void placeIt(int x, int y);

	virtual bool moveNorth();
	virtual bool moveEast();
	virtual bool moveSouth();
	virtual bool moveWest();

	virtual void levelUp() = 0;
	virtual void draw(WINDOW *win) = 0;

protected:
	virtual void checkCollisions(GameObject &game_object);
	virtual void doFOV();

	Race race_;
	char *str_race_;
	Map2D &map_;

	int health_points_;
	int attack_points_;
	int armor_points_;
	int experience_points_;
	int max_experience_points_;
	int level_;
	int explored_;

	Dungeon::Point location_;
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_H_
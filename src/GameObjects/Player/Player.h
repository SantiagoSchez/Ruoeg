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

	explicit Player(Race race);
	virtual ~Player();

	virtual Race race() const;
	virtual int health_points() const;
	virtual int attack_points() const;
	virtual int armor_points() const;
	virtual int experience_points() const;
	virtual int level() const;
	virtual bool isAlive() const;

	virtual const Dungeon::Point& location() const;
	virtual void placeIt(int x, int y);

	virtual bool moveNorth(Map2D &map);
	virtual bool moveEast(Map2D &map);
	virtual bool moveSouth(Map2D &map);
	virtual bool moveWest(Map2D &map);

	virtual void checkCollisions(GameObject &game_object);

	virtual void levelUp() = 0;
	virtual void draw(WINDOW *win) = 0;

protected:
	Race race_;

	int health_points_;
	int attack_points_;
	int armor_points_;
	int experience_points_;
	int level_;

	Dungeon::Point location_;
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_H_
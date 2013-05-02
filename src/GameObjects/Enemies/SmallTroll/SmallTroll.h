//
// A class representing a small troll in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_SMALLTROLL_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_SMALLTROLL_H_

#include "../Enemy.h"

class SmallTroll : public Enemy
{
public:
	SmallTroll(Dungeon &dungeon, int x, int y, int score, int exp);
	~SmallTroll();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_SMALLTROLL_H_
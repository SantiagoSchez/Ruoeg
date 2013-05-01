//
// A class representing a small goblin in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_SMALLGOBLIN_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_SMALLGOBLIN_H_

#include "../Enemy.h"

class SmallGoblin : public Enemy
{
public:
	SmallGoblin(Dungeon &dungeon, int x, int y);
	~SmallGoblin();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_SMALLGOBLIN_H_
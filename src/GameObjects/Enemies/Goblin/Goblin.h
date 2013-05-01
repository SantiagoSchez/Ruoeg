//
// A class representing a goblin in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_GOBLIN_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_GOBLIN_H_

#include "../Enemy.h"

class Goblin : public Enemy
{
public:
	Goblin(Dungeon &dungeon, int x, int y);
	~Goblin();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_GOBLIN_H_
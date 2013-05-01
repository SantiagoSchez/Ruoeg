//
// A class representing a troll in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_TROLL_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_TROLL_H_

#include "../Enemy.h"

class Troll : public Enemy
{
public:
	Troll(Dungeon &dungeon, int x, int y);
	~Troll();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_TROLL_H_
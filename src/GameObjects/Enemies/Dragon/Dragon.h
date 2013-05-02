//
// A class representing a dragon in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_DRAGON_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_DRAGON_H_

#include "../Enemy.h"

class Dragon : public Enemy
{
public:
	Dragon(Dungeon &dungeon, int x, int y, int score, int exp);
	~Dragon();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_DRAGON_H_
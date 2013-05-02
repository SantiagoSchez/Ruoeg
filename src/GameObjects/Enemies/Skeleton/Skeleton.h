//
// A class representing a skeleton in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_SKELETON_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_SKELETON_H_

#include "../Enemy.h"

class Skeleton : public Enemy
{
public:
	Skeleton(Dungeon &dungeon, int x, int y, int score, int exp);
	~Skeleton();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_SKELETON_H_
//
// A class representing a small skeleton in the map
// 

#ifndef RUOEG_GAMEOBJECTS_ENEMIES_SMALLSKELETON_H_
#define RUOEG_GAMEOBJECTS_ENEMIES_SMALLSKELETON_H_

#include "../Enemy.h"

class SmallSkeleton : public Enemy
{
public:
	SmallSkeleton();
	~SmallSkeleton();

	void levelUp();
};

#endif // RUOEG_GAMEOBJECTS_ENEMIES_SMALLSKELETON_H_
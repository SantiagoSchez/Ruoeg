//
// A class representing a chest in the map
// 

#ifndef RUOEG_GAMEOBJECTS_CHEST_H_
#define RUOEG_GAMEOBJECTS_CHEST_H_

#include "../GameObject.h"

class Chest : public GameObject
{
public:
	Chest(int score);
	~Chest();

	int score() const;

	static int num_chests();

private:
	int score_;
	static int num_chests_;
};

#endif // RUOEG_GAMEOBJECTS_CHEST_H_
//
// A class representing a chest in the map
// 

#ifndef RUOEG_GAMEOBJECTS_CHEST_H_
#define RUOEG_GAMEOBJECTS_CHEST_H_

#include "../GameObject.h"

class Chest : public GameObject
{
public:
	Chest();
	~Chest();

	static int num_chests();

private:
	static int num_chests_;
};

#endif // RUOEG_GAMEOBJECTS_CHEST_H_
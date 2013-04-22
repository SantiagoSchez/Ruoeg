//
// A class representing a door in the map
// 

#ifndef RUOEG_GAMEOBJECTS_DOOR_H_
#define RUOEG_GAMEOBJECTS_DOOR_H_

#include "../../GameObject.h"

class Door : public GameObject
{
public:
	Door();
	~Door();

	void open();
};

#endif // RUOEG_GAMEOBJECTS_DOOR_H_
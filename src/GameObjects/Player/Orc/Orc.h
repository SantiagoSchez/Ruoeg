//
// The Orc race
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_ORC_H_
#define RUOEG_GAMEOBJECTS_PLAYER_ORC_H_

#include "../Player.h"

class Orc : public Player
{
public:
	Orc(Dungeon &dungeon);
	~Orc();

private:
	void levelUp();
	void draw(WINDOW *win);
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_ORC_H_
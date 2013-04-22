//
// The Human race
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_HUMAN_H_
#define RUOEG_GAMEOBJECTS_PLAYER_HUMAN_H_

#include "../Player.h"

class Human : public Player
{
public:
	Human();
	~Human();

private:
	void levelUp();
	void draw(WINDOW *win);
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_HUMAN_H_
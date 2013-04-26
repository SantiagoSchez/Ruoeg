//
// The Elf race
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_ELF_H_
#define RUOEG_GAMEOBJECTS_PLAYER_ELF_H_

#include "../Player.h"

class Elf : public Player
{
public:
	Elf(Map2D &map);
	~Elf();

private:
	void levelUp();
	void draw(WINDOW *win);
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_ELF_H_
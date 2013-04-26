//
// The Dwarf race
// 

#ifndef RUOEG_GAMEOBJECTS_PLAYER_DWARF_H_
#define RUOEG_GAMEOBJECTS_PLAYER_DWARF_H_

#include "../Player.h"

class Dwarf : public Player
{
public:
	Dwarf(Map2D &map);
	~Dwarf();

private:
	void levelUp();
	void draw(WINDOW *win);
};

#endif // RUOEG_GAMEOBJECTS_PLAYER_DWARF_H_
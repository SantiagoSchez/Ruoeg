//
// A common dungeon in the game. It generates a randomly map.

#ifndef RUOEG_MAP_DUNGEON_H_
#define RUOEG_MAP_DUNGEON_H_

#include <vector>

#include "../Utils/RNG.h"
#include "../Crs/Crs.h"
#include "../Map/Map2D.h"

class Dungeon
{
public:
	Dungeon(unsigned int height, unsigned int width);
	~Dungeon();

	void draw(WINDOW *win);
	void generate();

private:
	struct Pair
	{
		unsigned int x;
		unsigned int y;
	};

	void makeRoom(Pair &location, Pair &size, GameObject &game_object);
	Pair getRandomWall();
	bool checkAdjacent(Pair &c, GameObject::Type type);

	RNG rng_;
	Map2D map_;
};

#endif // RUOEG_MAP_DUNGEON_H_
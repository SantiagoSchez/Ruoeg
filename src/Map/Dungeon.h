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
	enum class Direction
	{
		North,
		East,
		South,
		West
	};

	struct Pair
	{
		unsigned int x;
		unsigned int y;
	};

	bool makeRoom(Pair &loc, Pair &size, GameObject &game_object);
	bool makeCorridor(Pair &loc, unsigned int len, Direction dir);
	Pair getRandomWall();
	bool checkAdjacent(Pair &p, GameObject::Type type);

	RNG rng_;
	Map2D map_;
};

#endif // RUOEG_MAP_DUNGEON_H_
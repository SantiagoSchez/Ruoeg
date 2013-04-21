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
	Dungeon(int height, int width);
	~Dungeon();

	void draw(WINDOW *win);
	void generate();

	// Just for testing
	bool createCorridor();
	bool createSquaredRoom();

private:
	enum class Direction
	{
		None,
		North,
		East,
		South,
		West
	};

	struct Point
	{
		int x;
		int y;
		Direction dir;
		int xmod;
		int ymod;
	};

	bool makeSquaredRoom(Point &loc, int height, int width);
	bool makeCorridor(Point &loc, int len);
	Point getRandomWall();

	RNG rng_;
	Map2D map_;

	const int map_error;
};

#endif // RUOEG_MAP_DUNGEON_H_
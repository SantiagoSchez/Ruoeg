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

	// Just for testing
	bool createrCorridor();

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
		unsigned int x;
		unsigned int y;
		Direction dir;
	};

	bool makeRoom(Point &loc, Point &size, GameObject &game_object);
	bool makeCorridor(Point &loc, unsigned int len);
	Point getRandomWall();
	bool checkAdjacency(Point &p, GameObject::Type type);
	Direction checkDirection(Point &p, GameObject::Type type);

	RNG rng_;
	Map2D map_;
};

#endif // RUOEG_MAP_DUNGEON_H_
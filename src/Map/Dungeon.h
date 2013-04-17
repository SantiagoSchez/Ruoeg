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
	struct Room
	{
		unsigned int min_width_room, min_height_room;
		unsigned int max_width_room, max_height_room;
	};

	struct Coordinate
	{
		unsigned int row, column;
	};

	void createRandomRoom(Room &r, GameObject &game_object);
	Coordinate getRandomWall();
	bool checkAdjacent(Coordinate &c, GameObject::Type type);

	RNG rng_;
	Map2D map_;
};

#endif // RUOEG_MAP_DUNGEON_H_
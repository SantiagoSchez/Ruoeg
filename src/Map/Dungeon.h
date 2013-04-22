//
// A common dungeon in the game. It generates a randomly map.

#ifndef RUOEG_MAP_DUNGEON_H_
#define RUOEG_MAP_DUNGEON_H_

#include <vector>

#include "../Utils/RNG.h"
#include "../Curses/Curses.h"
#include "../Map/Map2D.h"

class Dungeon
{
public:
	enum class Direction
	{
		None,
		North,
		East,
		South,
		West,
	};

	struct Point
	{
		int x;
		int y;
		Direction dir;
		// Variables used to modify the prior ones when building a 
		// connection between features i.e. a door/lit.
		int x_mod;
		int y_mod;
	};

	Dungeon(int height, int width);
	~Dungeon();

	void draw(WINDOW *win);
	void generate();

	Map2D& map();

	// Some getters for get the number of features
	int num_rooms() const;
	int num_corridors() const;
	int num_enemies() const;
	int num_chests() const;

	// Helper methods
	Point getRandomWall();
	Point getRandomLit();
	Point getRandomCorridor();

private:
	// Features to build. More can be added here.
	bool makeSquaredRoom(Point &loc, int height, int width);
	bool makeCorridor(Point &loc, int len);

	// Spawns a GameObject in the given location and pushes back
	// in the top of the tile
	bool spawn(int row, int column, GameObject &game_object);
	bool spawn(Point &p, GameObject &game_object, int offset);

	// Draw the GameObject on the given position, on the given window
	// and with the given color (1..n)
	void drawGameObject(WINDOW *win, int y, int x, GameObject::Type g, GameObject::Color color);

	// Check whether there are objects of the given type around the
	// given location within the given radius.
	bool checkObjectsSurrounding(Point &loc, GameObject::Type type, int radius);

	// Check corners of rooms for not building
	bool checkCorners(Point &loc);

	// The random number generator object.
	RNG rng_;

	// The internal object to keep the dungeon's map.
	Map2D map_;

	// Information variables about the features.
	int num_rooms_;
	int num_corridors_;
	int num_enemies_;
	int num_chests_;

	// Some min values
	const int min_room_height;
	const int min_room_width;

	// This is a margin of error that allows the map to be fitted
	// inside the window avoiding problems of index out of boundaries.
	// Currently it's set to 4 i.e. (4, 4, height-4, width-4).
	const int map_error;

	// This is a variable used to save the number of dungeons 
	// (or level of dungeon) we have generated. We'll use it to take 
	// care the monsters are stronger at higher level.
	static int dungeon_level;
};

#endif // RUOEG_MAP_DUNGEON_H_
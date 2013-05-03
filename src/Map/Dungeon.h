//
// A common dungeon in the game. It generates a randomly map.

#ifndef RUOEG_MAP_DUNGEON_H_
#define RUOEG_MAP_DUNGEON_H_

#include "../Utils/RNG.h"
#include "../Utils/Point.h"
#include "../Utils/Direction.h"
#include "../Curses/Curses.h"
#include "../Map/Map2D.h"
#include "../GameObjects/Enemies/Enemy.h"

#include <vector>

typedef std::shared_ptr<Enemy> EnemyPtr;

class Dungeon
{
public:
	Dungeon(int height, int width);
	~Dungeon();

	void draw(WINDOW *win);
	void generate();

	Map2D& map();

	std::vector<EnemyPtr>& enemies();

	// Some getters for the features
	int num_rooms() const;
	int num_corridors() const;
	int num_enemies() const;
	int floor() const;
	void reset_floor();
	const Point& stairs_location() const;

	// Helper methods
	Point getRandomWall();
	Point getRandomLit();
	Point getRandomCorridor();

private:
	// When we want to restart the class
	void restart();

	// Features to build. More can be added here.
	bool makeSquaredRoom(Point &loc, int height, int width);
	bool makeCorridor(Point &loc, int len);

	// Spawns a GameObject in the given location and pushes back
	// in the top of the tile
	bool spawn(int row, int column, GameObjectPtr game_object);
	bool spawnEnemy(int row, int column, GameObjectPtr game_object);
	bool spawn(Point &p, GameObjectPtr game_object, int offset);
	bool spawnEnemy(Point &p, EnemyPtr game_object, int offset);

	// Check whether there are objects of the given type around the
	// given location within the given radius.
	bool checkObjectsSurrounding(Point &loc, GameObject::Type type, int radius);

	// Check corners of rooms for not building
	bool checkCorners(Point &loc);

	// The random number generator object.
	RNG rng_;

	// The internal object to keep the dungeon's map.
	Map2D map_;

	// A list of enemies.
	std::vector<EnemyPtr> enemies_;

	// Information variables about the features.
	int num_rooms_;
	int num_corridors_;
	int num_enemies_;

	// Current floor
	int floor_;

	// Location for the stairs
	Point stairs_location_;

	// Some min values
	const int min_room_height;
	const int min_room_width;

	// This is a margin of error that allows the map to be fitted
	// inside the window avoiding problems of index out of boundaries.
	// Currently it's set to 4 i.e. (4, 4, height-4, width-4).
	const int map_error;
};

#endif // RUOEG_MAP_DUNGEON_H_
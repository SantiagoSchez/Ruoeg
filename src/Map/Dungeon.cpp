#include "Dungeon.h"

#include "../GameObjects/Wall.h"
#include "../GameObjects/Lit.h"

Dungeon::Dungeon(unsigned int height, unsigned int width) 
	: map_(height, width)
{
	generate();
}

Dungeon::~Dungeon()
{
}

void Dungeon::generate()
{
	/*
	 * A Roguebasin's based algorithm. 
	 * Each step is enumerated to keep it clear.
	 */

	map_.clear();

	// Fill the whole map with solid earth
	unsigned int size = map_.width() * map_.height();
	for(unsigned int i = 0; i < size; ++i)
	{
		map_.at(i).add(Wall());
	}
	
	// Dig out a single room in the center of the map
	Room room = {4, 3, 7, 6}; // Min and Max size
	createRandomRoom(room, Lit());

	// Pick a wall of any room
	Coordinate p = getRandomWall();
	map_.at(p.row, p.column).top() = GameObject(GameObject::Type::Dummy); // TODO this is just a dummy!!!

	// Decide upon a new feature to build

}

void Dungeon::createRandomRoom(Room &r, GameObject &game_object)
{
	// Using Mersenne Twister engine for random numbers
	unsigned int row = rng_.nextInt(3, map_.height()-r.max_height_room-3);
	unsigned int column = rng_.nextInt(3, map_.width()-r.max_width_room-3);
	unsigned int height = row + 
		rng_.nextInt(r.min_height_room, r.max_height_room);
	unsigned int width = column + 
		rng_.nextInt(r.min_width_room, r.max_width_room);

	for(unsigned int i = row; i < height; ++i)
	{
		for(unsigned int j = column; j < width; ++j)
		{
			map_.at(i, j).top() = game_object;
		}
	}
}

Dungeon::Coordinate Dungeon::getRandomWall()
{
	Coordinate c; 
	bool ok = false;
	while(!ok)
	{
		c.row = rng_.nextInt(2, map_.height()-2);
		c.column = rng_.nextInt(2, map_.width()-2);
		Tile t1 = map_.at(c.row, c.column);

		if(!t1.visited())
		{
			// Mark it as visited
			t1.set_visited(true);

			// Check for tiles adjacent
			if(t1.top().type() == GameObject::Type::Wall)
			{
				ok = checkAdjacent(c, GameObject::Type::Lit);
			}
		}
	}

	return c;
}

bool Dungeon::checkAdjacent(Coordinate &c, GameObject::Type type)
{
	if((map_.at(c.row-1, c.column).top().type() == type) || 
		(map_.at(c.row+1, c.column).top().type() == type) ||
		(map_.at(c.row, c.column-1).top().type() == type) ||
		(map_.at(c.row,c. column+1).top().type() == type))
	{
		return true;
	}
	
	return false;
}

void Dungeon::draw(WINDOW *win)
{
	unsigned int height = map_.height();
	unsigned int width = map_.width();

	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			Crs::mvwaddch(win, i, j, static_cast<char>(map_.at(i, j).top().type()));
		}
	}
}
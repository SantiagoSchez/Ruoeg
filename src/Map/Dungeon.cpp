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
	// A Roguebasin's based algorithm. 
	// Each step is enumerated to keep it clear.

	map_.clear();

	// Fill the whole map with solid earth
	unsigned int size = map_.width() * map_.height();
	for(unsigned int i = 0; i < size; ++i)
	{
		map_.at(i).add(Wall());
	}
	
	// Dig out a single room in the center of the map
	Pair location = {map_.width()/2, map_.height()/2};
	Pair dimensions = {8, 6};
	makeRoom(location, dimensions, Lit());

	// Pick a wall of any room
	Pair p = getRandomWall();
	map_.at(p.y, p.x).top() = GameObject(GameObject::Type::Dummy); // TODO this is just a dummy!!!

	// Decide upon a new feature to build

}

void Dungeon::makeRoom(Pair &location, Pair &size, GameObject &game_object)
{
	// Using Mersenne Twister engine for random numbers
	unsigned int row = location.y - size.y/2;
	unsigned int column = location.x - size.x/2;
	unsigned int height = location.y + rng_.nextInt(4, size.y);
	unsigned int width = location.x + rng_.nextInt(4, size.x);

	for(unsigned int i = row; i < height; ++i)
	{
		for(unsigned int j = column; j < width; ++j)
		{
			map_.at(i, j).top() = game_object;
		}
	}
}

Dungeon::Pair Dungeon::getRandomWall()
{
	Pair p; 
	bool ok = false;
	while(!ok)
	{
		p.y = rng_.nextInt(2, map_.height()-2);
		p.x = rng_.nextInt(2, map_.width()-2);
		Tile t1 = map_.at(p.y, p.x);

		if(!t1.visited())
		{
			// Mark it as visited
			t1.set_visited(true);

			// Check for tiles adjacent
			if(t1.top().type() == GameObject::Type::Wall)
			{
				ok = checkAdjacent(p, GameObject::Type::Lit);
			}
		}
	}

	return p;
}

bool Dungeon::checkAdjacent(Pair &p, GameObject::Type type)
{
	if((map_.at(p.y-1, p.x).top().type() == type) || 
		(map_.at(p.y+1, p.x).top().type() == type) ||
		(map_.at(p.y, p.x-1).top().type() == type) ||
		(map_.at(p.y, p.x+1).top().type() == type))
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
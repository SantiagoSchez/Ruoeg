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

	int num_features = 0;
	while(num_features < 5) // Decide about the max
	{
		// Pick a wall of any room
		Pair p = getRandomWall();

		// Decide upon a new feature to build
		makeCorridor(p, 6, Direction::North);
		makeCorridor(p, 6, Direction::South);

		++num_features;
	}
}

bool Dungeon::makeRoom(Pair &loc, Pair &size, GameObject &game_object)
{
	// Using Mersenne Twister engine for random numbers
	unsigned int row = loc.y - size.y/2;
	unsigned int column = loc.x - size.x/2;
	unsigned int height = loc.y + rng_.nextInt(4, size.y);
	unsigned int width = loc.x + rng_.nextInt(4, size.x);

	for(unsigned int i = row; i < height; ++i)
	{
		for(unsigned int j = column; j < width; ++j)
		{
			map_.at(i, j).top() = game_object;
		}
	}

	return true;
}

bool Dungeon::makeCorridor(Pair &loc, unsigned int len, Direction dir)
{
	unsigned int length = rng_.nextInt(2, len);
	unsigned int width = map_.width();
	unsigned int height = map_.height();

	switch(dir)
	{
	case Direction::North:
		for(unsigned int i = loc.y; i > (loc.y-length); --i)
		{
			if((i < 0) || (i > height) ||
			   (map_.at(i, loc.x).top().type() != GameObject::Type::Wall)) 
			{
				return false;
			}
		}

		for(unsigned int i = loc.y; i > (loc.y-length); --i)
		{
			map_.at(i, loc.x).top() = Lit();
		}
		break;
	case Direction::East:
		break;
	case Direction::South:
		for(unsigned int i = loc.y; i < (loc.y+length); ++i)
		{
			if((i < 0) || (i > height) ||
				(map_.at(i, loc.x).top().type() != GameObject::Type::Wall)) 
			{
				return false;
			}
		}

		for(unsigned int i = loc.y; i < (loc.y+length); ++i)
		{
			map_.at(i, loc.x).top() = Lit();
		}
		break;
	case Direction::West:
		break;
	}

	return true;
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
			Crs::mvwaddch(win, i, j, 
				static_cast<char>(map_.at(i, j).top().type()));
		}
	}
}
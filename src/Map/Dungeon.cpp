#include "Dungeon.h"

#include <iostream>

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
	Point location = {map_.width()/2, map_.height()/2};
	Point dimensions = {8, 6};
	makeRoom(location, dimensions, Lit());

	int num_features = 0;
	while(num_features < 10000) // Decide about the max
	{
		// Pick a wall of any room
		Point p = getRandomWall();

		// Decide upon a new feature to build
		//makeCorridor(p, 6, Direction::North);
		//makeCorridor(p, 6, Direction::South);
		makeCorridor(p, 6);

		++num_features;
	}
}

bool Dungeon::makeRoom(Point &loc, Point &size, GameObject &game_object)
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

bool Dungeon::makeCorridor(Point &loc, unsigned int len)
{
	unsigned int length = rng_.nextInt(5, len);
	unsigned int width = map_.width();
	unsigned int height = map_.height();

	switch(loc.dir)
	{
	case Direction::North:
		for(unsigned int i = loc.y; i > (loc.y-length); --i)
		{
			if((i < 2) || (i > height-2) ||
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
		for(unsigned int i = loc.x; i < (loc.x+length); ++i)
		{
			if((i < 2) || (i > width-2) ||
				(map_.at(loc.y, i).top().type() != GameObject::Type::Wall)) 
			{
				return false;
			}
		}

		for(unsigned int i = loc.x; i < (loc.x+length); ++i)
		{
			map_.at(loc.y, i).top() = Lit();
		}
		break;
	case Direction::South:
		for(unsigned int i = loc.y; i < (loc.y+length); ++i)
		{
			if((i < 2) || (i > height-2) ||
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
		for(unsigned int i = loc.x; i > (loc.x-length); --i)
		{
			if((i < 2) || (i > width-2) ||
				(map_.at(loc.y, i).top().type() != GameObject::Type::Wall)) 
			{
				return false;
			}
		}

		for(unsigned int i = loc.x; i > (loc.x-length); --i)
		{
			map_.at(loc.y, i).top() = Lit();
		}
		break;
	}

	return true;
}

Dungeon::Point Dungeon::getRandomWall()
{
	Point p; 
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
				ok = checkAdjacency(p, GameObject::Type::Lit);
				if(ok)
				{
					p.dir = checkDirection(p, GameObject::Type::Wall);
				}
			}
		}
	}

	return p;
}

bool Dungeon::checkAdjacency(Point &p, GameObject::Type type)
{
	bool free = false;

	if((map_.at(p.y-1, p.x).top().type() == type) || 
	   (map_.at(p.y+1, p.x).top().type() == type) ||
	   (map_.at(p.y, p.x-1).top().type() == type) ||
	   (map_.at(p.y, p.x+1).top().type() == type))
	{
		free = true;
	}
	
	return free;
}

Dungeon::Direction Dungeon::checkDirection(Point &p, GameObject::Type type)
{
	Direction dir = Direction::None;

	if(map_.at(p.y-1, p.x).top().type() == type)
	{
		if((map_.at(p.y, p.x-1).top().type() == type) && 
			(map_.at(p.y, p.x+1).top().type() == type))
		{
			dir = Direction::North;
		}
	}

	if(map_.at(p.y+1, p.x).top().type() == type)
	{
		if((map_.at(p.y, p.x-1).top().type() == type) && 
			(map_.at(p.y, p.x+1).top().type() == type))
		{
			dir = Direction::South;
		}
	}

	if(map_.at(p.y, p.x+1).top().type() == type)
	{
		if((map_.at(p.y-1, p.x).top().type() == type) && 
			(map_.at(p.y+1, p.x).top().type() == type))
		{
			dir = Direction::East;
		}
	}

	if(map_.at(p.y, p.x-1).top().type() == type)
	{
		if((map_.at(p.y-1, p.x).top().type() == type) && 
			(map_.at(p.y+1, p.x).top().type() == type))
		{
			dir = Direction::West;
		}
	}

	return dir;
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
#include "Dungeon.h"

#include <iostream>

#include "../GameObjects/Wall.h"
#include "../GameObjects/Lit.h"
#include "../GameObjects/Dummy.h"

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
	Point location = {
		map_.width()/2, 
		map_.height()/2, 
		static_cast<Direction>(rng_.nextInt(1, 4))
	};
	makeSquaredRoom(location, 6, 8);

	int num_features = 0;
	while(num_features < 1000) // Decide about the max
	{
		// Pick a wall of any room
		Point p = getRandomWall();

		// Decide upon a new feature to build
		if(rng_.nextInt(0, 100) <= 75)
		{
			makeCorridor(p, 6);
		}
		else
		{
			makeSquaredRoom(p, 6, 8);
		}

		++num_features;
	}
}

bool Dungeon::makeSquaredRoom(Point &loc, unsigned int height, unsigned int width)
{
	unsigned int real_height = rng_.nextInt(4, height);
	unsigned int real_width = rng_.nextInt(4, width);

	switch(loc.dir)
	{
	case Direction::North:
		//Check if there's enough space left for it
		for(unsigned int i = loc.y; i > (loc.y-real_height); --i)
		{
			if (i < 2 || i > map_.height()-2)
			{
				return false;
			}
			for(unsigned int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if(j < 2 || j > map_.width()-2) 
				{
					return false;
				}
				if(map_.at(i, j).top().type() != GameObject::Type::Wall) 
				{
					return false; //no space left...
				}
			}
		}

		// All went ok? Then build the squared room
		for(unsigned int i = loc.y; i > (loc.y-real_height); --i)
		{
			for(unsigned int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if (j == (loc.x-real_width/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == loc.y)
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == (loc.y-real_height+1))
				{
					map_.at(i, j).top() = Wall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::East:
		for(unsigned int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			if(i < 2 || i > map_.height()-2)
			{
				return false;
			}
			for(unsigned int j = loc.x; j < (loc.x+real_width); ++j)
			{
				if(j < 2 || j > map_.width()-2)
				{
					return false;
				}
				if(map_.at(i, j).top().type() != GameObject::Type::Wall) 
				{
					return false;
				}
			}
		}

		for(unsigned int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			for(unsigned int j = loc.x; j < (loc.x+real_width); ++j)
			{
				if(j == loc.x)
				{
					map_.at(i, j).top() = Wall();
				}
				else if(j == (loc.x+real_width-1))
				{
					map_.at(i, j).top() = Wall();;
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::South:
		for(unsigned int i = loc.y; i < (loc.y+real_height); ++i)
		{
			if (i < 2 || i > map_.height()-2)
			{
				return false;
			}
			for(unsigned int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if(j < 2 || j > map_.width()-2) 
				{
					return false;
				}
				if(map_.at(i, j).top().type()!=GameObject::Type::Wall) 
				{
					return false;
				}
			}
		}

		for(unsigned int i = loc.y; i < (loc.y+real_height); ++i)
		{
			for(unsigned int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if (j == (loc.x-real_width/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == loc.y)
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == (loc.y+real_height-1))
				{
					map_.at(i, j).top() = Wall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::West:
		for(unsigned int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			if(i < 2 || i > map_.height()-2)
			{
				return false;
			}
			for(unsigned int j = loc.x; j > (loc.x-real_width); --j)
			{
				if(j < 2 || j > map_.width()-2)
				{
					return false;
				}
				if(map_.at(i, j).top().type() != GameObject::Type::Wall) 
				{
					return false;
				}
			}
		}

		for(unsigned int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			for(unsigned int j = loc.x; j > (loc.x-real_width); --j)
			{
				if(j == loc.x)
				{
					map_.at(i, j).top() = Wall();
				}
				else if(j == (loc.x-real_width+1))
				{
					map_.at(i, j).top() = Wall();;
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).top() = Wall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	}

	map_.at(loc.y, loc.x).top() = Lit();

	return true;
}

bool Dungeon::makeCorridor(Point &loc, unsigned int len)
{
	unsigned int length = rng_.nextInt(4, len);
	unsigned int width = map_.width();
	unsigned int height = map_.height();

	switch(loc.dir)
	{
	case Direction::North:
		// Checking for free area to build the corridor; the corridor
		// itself and the adjacent sides
		// #####**********		###############
		// #...#W*********	->	#...#..........
		// #####**********		###############
		for(unsigned int i = loc.y; i > (loc.y-length); --i)
		{
			if((i < 2) || 
			   (i > height-2) ||
			   (map_.at(i, loc.x).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(i, loc.x-1).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(i, loc.x+1).top().type() != GameObject::Type::Wall)) 
			{
				return false;
			}
		}

		// All went ok? Then build the corridor
		for(unsigned int i = loc.y; i > (loc.y-length); --i)
		{
			map_.at(i, loc.x).top() = Lit();
		}
		break;
	case Direction::East:
		for(unsigned int i = loc.x; i < (loc.x+length); ++i)
		{
			if((i < 2) || 
			   (i > width-2) ||
			   (map_.at(loc.y, i).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(loc.y-1, i).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(loc.y+1, i).top().type()!=GameObject::Type::Wall)) 
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
			if((i < 2) || 
			   (i > height-2) ||
			   (map_.at(i, loc.x).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(i, loc.x-1).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(i, loc.x+1).top().type()!=GameObject::Type::Wall)) 
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
			if((i < 2) || 
			   (i > width-2) ||
			   (map_.at(loc.y, i).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(loc.y-1, i).top().type()!=GameObject::Type::Wall) ||
			   (map_.at(loc.y+1, i).top().type()!=GameObject::Type::Wall)) 
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
				p.dir = checkAdjacency(p, GameObject::Type::Lit);
				if(p.dir != Direction::None)
				{
					ok = true;
				}
			}
		}
	}

	return p;
}

Dungeon::Direction Dungeon::checkAdjacency(Point &p, GameObject::Type type)
{
	Direction dir = Direction::None;

	// Check for if we can reach the place
	if(map_.at(p.y+1, p.x).top().type() == type)
	{
		dir = Direction::North;
	}
	else if(map_.at(p.y, p.x-1).top().type() == type)
	{
		dir = Direction::East;
	}
	else if(map_.at(p.y-1, p.x).top().type() == type)
	{
		dir = Direction::South;
	}
	else if(map_.at(p.y, p.x+1).top().type() == type)
	{
		dir = Direction::West;
	}
	
	// Check for doors nearby
	/*if(dir != Direction::None)
	{
		if (getCell(newx, newy+1) == tileDoor) //north
			validTile = -1;
		else if (getCell(newx-1, newy) == tileDoor)//east
			validTile = -1;
		else if (getCell(newx, newy-1) == tileDoor)//south
			validTile = -1;
		else if (getCell(newx+1, newy) == tileDoor)//west
			validTile = -1;
	}*/
	
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

bool Dungeon::createCorridor()
{
	Point p = getRandomWall();
	return makeCorridor(p, 6);
}

bool Dungeon::createSquaredRoom()
{
	Point l = getRandomWall();
	return makeSquaredRoom(l, 6, 8);
}

#include "Dungeon.h"

#include <iostream>

#include "../GameObjects/Wall/HorizontalWall.h"
#include "../GameObjects/Wall/VerticalWall.h"
#include "../GameObjects/Corridor/Corridor.h"
#include "../GameObjects/Lit/Lit.h"
#include "../GameObjects/Door/Door.h"
#include "../GameObjects/None/None.h"

Dungeon::Dungeon(int height, int width) 
	: map_(height, width), map_error(4)
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
	int size = map_.width() * map_.height();
	for(int i = 0; i < size; ++i)
	{
		map_.at(i).add(None());
	}
	
	// Dig out a single room in the center of the map
	Point location = {
		map_.width()/2, 
		map_.height()/2, 
		static_cast<Direction>(rng_.nextInt(1, 4))
	};
	makeSquaredRoom(location, 6, 8);

	for(int num_features = 0; num_features < 1000; ++num_features)
	{
		// Pick a wall of any room
		Point p = getRandomWall();
		p.x += p.xmod;
		p.y += p.ymod;

		// Decide upon a new feature to build
		if(rng_.nextInt(0, 100) <= 75)
		{
			if(makeSquaredRoom(p, 6, 8)) // 75% for a room
			{
				map_.at(p.y-p.ymod, p.x-p.xmod).top() = Door();
				map_.at(p.y, p.x).top() = Lit();
			}
		}
		else
		{
			if(makeCorridor(p, 6)) // 25% for a corridor
			{
				map_.at(p.y-p.ymod, p.x-p.xmod).top() = Door();
			}
		}
	}
}

bool Dungeon::makeSquaredRoom(Point &loc, int height, int width)
{
	int real_height = rng_.nextInt(4, height);
	int real_width = rng_.nextInt(4, width);

	switch(loc.dir)
	{
	case Direction::North:
		// Check if there's enough space left for it
		for(int i = loc.y; i > (loc.y-real_height); --i)
		{
			if(i < map_error || i > map_.height()-map_error)
			{
				return false;
			}
			for(int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if((j < map_error) || 
				   (j > map_.width()-map_error) ||
				   (map_.at(i, j).top().type() != GameObject::Type::None))
				{
					return false;
				}
			}
		}

		// All went ok? Then build the squared room
		for(int i = loc.y; i > (loc.y-real_height); --i)
		{
			for(int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if (j == (loc.x-real_width/2))
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(i == loc.y)
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else if(i == (loc.y-real_height+1))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::East:
		for(int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			if(i < map_error || i > map_.height()-map_error)
			{
				return false;
			}
			for(int j = loc.x; j < (loc.x+real_width); ++j)
			{
				if((j < map_error) || 
				   (j > map_.width()-map_error) ||
				   (map_.at(i, j).top().type() != GameObject::Type::None))
				{
					return false;
				}
			}
		}

		for(int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			for(int j = loc.x; j < (loc.x+real_width); ++j)
			{
				if(j == loc.x)
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(j == (loc.x+real_width-1))
				{
					map_.at(i, j).top() = VerticalWall();;
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::South:
		for(int i = loc.y; i < (loc.y+real_height); ++i)
		{
			if (i < map_error || i > map_.height()-map_error)
			{
				return false;
			}
			for(int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if((j < map_error) ||
				   (j > map_.width()-map_error) ||
				   (map_.at(i, j).top().type() != GameObject::Type::None)) 
				{
					return false;
				}
			}
		}

		for(int i = loc.y; i < (loc.y+real_height); ++i)
		{
			for(int j = (loc.x-real_width/2); j < (loc.x+(real_width+1)/2); ++j)
			{
				if (j == (loc.x-real_width/2))
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(i == loc.y)
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else if(i == (loc.y+real_height-1))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	case Direction::West:
		for(int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			if(i < map_error || i > map_.height()-map_error)
			{
				return false;
			}
			for(int j = loc.x; j > (loc.x-real_width); --j)
			{
				if((j < map_error) || 
				   (j > map_.width()-map_error) ||
				   (map_.at(i, j).top().type() != GameObject::Type::None))
				{
					return false;
				}
			}
		}

		for(int i = (loc.y-real_height/2); i < (loc.y+(real_height+1)/2); ++i)
		{
			for(int j = loc.x; j > (loc.x-real_width); --j)
			{
				if(j == loc.x)
				{
					map_.at(i, j).top() = VerticalWall();
				}
				else if(j == (loc.x-real_width+1))
				{
					map_.at(i, j).top() = VerticalWall();;
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).top() = HorizontalWall();
				}
				else 
				{
					map_.at(i, j).top() = Lit();
				}
			}
		}
		break;
	}

	return true;
}

bool Dungeon::makeCorridor(Point &loc, int len)
{
	int length = rng_.nextInt(4, len);

	switch(loc.dir)
	{
	case Direction::North:
		// Checking for free area to build the corridor
		for(int i = loc.y; i > (loc.y-length); --i)
		{
			if((i < map_error) || 
			   (i > map_.height()-map_error) ||
			   (map_.at(i, loc.x).top().type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		// All went ok? Then build the corridor
		for(int i = loc.y; i > (loc.y-length); --i)
		{
			map_.at(i, loc.x).top() = Corridor();
		}
		break;
	case Direction::East:
		for(int i = loc.x; i < (loc.x+length); ++i)
		{
			if((i < map_error) || 
			   (i > map_.width()-map_error) ||
			   (map_.at(loc.y, i).top().type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.x; i < (loc.x+length); ++i)
		{
			map_.at(loc.y, i).top() = Corridor();
		}
		break;
	case Direction::South:
		for(int i = loc.y; i < (loc.y+length); ++i)
		{
			if((i < map_error) || 
			   (i > map_.height()-map_error) ||
			   (map_.at(i, loc.x).top().type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.y; i < (loc.y+length); ++i)
		{
			map_.at(i, loc.x).top() = Corridor();
		}
		break;
	case Direction::West:
		for(int i = loc.x; i > (loc.x-length); --i)
		{
			if((i < map_error) || 
			   (i > map_.width()-map_error) ||
			   (map_.at(loc.y, i).top().type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.x; i > (loc.x-length); --i)
		{
			map_.at(loc.y, i).top() = Corridor();
		}
		break;
	}

	return true;
}

Dungeon::Point Dungeon::getRandomWall()
{
	Point p;
	p.dir = Direction::None;
	p.xmod = 0;
	p.ymod = 0;

	bool ok = false;
	while(!ok) 
	{
		p.y = rng_.nextInt(2, map_.height()-2);
		p.x = rng_.nextInt(2, map_.width()-2);

		if((map_.at(p.y, p.x).top().type() == GameObject::Type::HorizontalWall) ||
		   (map_.at(p.y, p.x).top().type() == GameObject::Type::VerticalWall) ||
		   (map_.at(p.y, p.x).top().type() == GameObject::Type::Corridor))
		{
			// Check if we can reach the place
			if((map_.at(p.y+1, p.x).top().type() == GameObject::Type::Lit) ||
			   (map_.at(p.y+1, p.x).top().type() == GameObject::Type::Corridor))
			{
				p.dir = Direction::North;
				p.xmod = 0;
				p.ymod = -1;
			}
			else if((map_.at(p.y, p.x-1).top().type() == GameObject::Type::Lit) ||
				    (map_.at(p.y, p.x-1).top().type() == GameObject::Type::Corridor))
			{
				p.dir = Direction::East;
				p.xmod = +1;
				p.ymod = 0;
			}
			else if((map_.at(p.y-1, p.x).top().type() == GameObject::Type::Lit) ||
				    (map_.at(p.y-1, p.x).top().type() == GameObject::Type::Corridor))
			{
				p.dir = Direction::South;
				p.xmod = 0;
				p.ymod = +1;
			}
			else if((map_.at(p.y, p.x+1).top().type() == GameObject::Type::Lit) ||
				    (map_.at(p.y, p.x+1).top().type() == GameObject::Type::Corridor))
			{
				p.dir = Direction::West;
				p.xmod = -1;
				p.ymod = 0;
			}

			// Check for nearby doors
			if(p.dir != Direction::None)
			{
				if((map_.at(p.y+1, p.x).top().type() == GameObject::Type::Door) ||
				   (map_.at(p.y, p.x-1).top().type() == GameObject::Type::Door) ||
				   (map_.at(p.y-1, p.x).top().type() == GameObject::Type::Door) ||
				   (map_.at(p.y, p.x+1).top().type() == GameObject::Type::Door))
				{
					p.dir = Direction::None;
				}
			}

			if(p.dir != Direction::None)
			{
				ok = true;
			}
		}
	}

	return p;
}

void Dungeon::draw(WINDOW *win)
{
	int height = map_.height();
	int width = map_.width();

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			GameObject::Type g = map_.at(i, j).top().type();

			switch(g)
			{
			case GameObject::Type::Door:
				Crs::wattron(win, COLOR_PAIR(4));
				Crs::mvwaddch(win, i, j, static_cast<char>(g));
				Crs::wattroff(win, COLOR_PAIR(4));
				break;
			case GameObject::Type::Corridor:
			case GameObject::Type::Lit:
				Crs::wattron(win, COLOR_PAIR(2));
				Crs::mvwaddch(win, i, j, static_cast<char>(g));
				Crs::wattroff(win, COLOR_PAIR(2));
				break;
			case GameObject::Type::HorizontalWall:
			case GameObject::Type::VerticalWall:
				Crs::wattron(win, COLOR_PAIR(1));
				Crs::mvwaddch(win, i, j, static_cast<char>(g));
				Crs::wattroff(win, COLOR_PAIR(1));
				break;
			default:
				Crs::mvwaddch(win, i, j, static_cast<char>(g));
			}
		}
	}
}

bool Dungeon::createCorridor()
{
	Point p = getRandomWall();
	p.x += p.xmod;
	p.y += p.ymod;

	if(makeCorridor(p, 6))
	{
		map_.at(p.y-p.ymod, p.x-p.xmod).top() = Door();

		return true;
	}

	return false;
}

bool Dungeon::createSquaredRoom()
{
	Point p = getRandomWall();
	p.x += p.xmod;
	p.y += p.ymod;

	if(makeSquaredRoom(p, 6, 8))
	{
		map_.at(p.y-p.ymod, p.x-p.xmod).top() = Door();
		map_.at(p.y, p.x).top() = Lit();

		return true;
	}

	return false;
}

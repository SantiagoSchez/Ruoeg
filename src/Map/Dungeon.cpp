#include "Dungeon.h"

#include <iostream>
#include <memory>

#include "../Game/Game.h"
#include "../GameObjects/Terrains/Wall/HorizontalWall.h"
#include "../GameObjects/Terrains/Wall/VerticalWall.h"
#include "../GameObjects/Terrains/Corridor/Corridor.h"
#include "../GameObjects/Terrains/Lit/Lit.h"
#include "../GameObjects/Terrains/Door/Door.h"
#include "../GameObjects/Terrains/None/None.h"
#include "../GameObjects/Chests/Chest.h"
#include "../GameObjects/Chests/MapItem.h"
#include "../GameObjects/Chests/CompassItem.h"
#include "../GameObjects/Enemies/SmallDragon/SmallDragon.h"
#include "../GameObjects/Enemies/SmallGoblin/SmallGoblin.h"
#include "../GameObjects/Enemies/SmallSkeleton/SmallSkeleton.h"
#include "../GameObjects/Enemies/SmallTroll/SmallTroll.h"
#include "../GameObjects/Enemies/Dragon/Dragon.h"
#include "../GameObjects/Enemies/Goblin/Goblin.h"
#include "../GameObjects/Enemies/Skeleton/Skeleton.h"
#include "../GameObjects/Enemies/Troll/Troll.h"
#include "../GameObjects/Terrains/Stairs/Stairs.h"

Dungeon::Dungeon(int height, int width) 
	: map_(height, width), map_error(4), min_room_height(4), 
	  min_room_width(4), num_rooms_(0), num_corridors_(0), num_enemies_(0),
	  floor_(0)
{
}

Dungeon::~Dungeon()
{
}

void Dungeon::generate()
{
	// Dungeon generation based on next algorithm:
	// http://roguebasin.roguelikedevelopment.org/index.php?title=Dungeon-Building_Algorithm

	// Erase the previous dungeon
	Curses::werase(Game::getInstance().mapWindow());

	// Restart map stuff
	restart();

	// Increase the current level of the dungeon
	++floor_;

	// Fill the whole map with solid earth
	int size = map_.width() * map_.height();
	for(int i = 0; i < size; ++i)
	{
		map_.at(i).add(std::make_shared<None>());
	}
	
	// Dig out a single room in the center of the map
	Point location(map_.width()/2, map_.height()/2);
	location.dir = static_cast<Direction>(rng_.nextInt(1, 4));
	makeSquaredRoom(location, 6, 8);

	// Build some features. It will try 1000 times to add the features
	for(int num_features = 0; num_features < 1000; ++num_features)
	{
		// Pick a random wall
		Point p = getRandomWall();
		if(p.x == -900)
		{
			continue;
		}

		p.x += p.x_mod;
		p.y += p.y_mod;

		// Decide upon a new feature to build
		if(rng_.nextInt(0, 100) <= 75)
		{
			if(makeSquaredRoom(p, 6, 8)) // 75% for a room
			{
				*(map_.at(p.y-p.y_mod, p.x-p.x_mod).top()) = Door();
				*(map_.at(p.y, p.x).top()) = Lit();
			}
		}
		else
		{
			if(makeCorridor(p, 6)) // 25% for a corridor
			{
				*(map_.at(p.y-p.y_mod, p.x-p.x_mod).top()) = Door();
			}
		}
	}

	// Generate a boss enemy
	int chance = rng_.nextInt(0, 3);
	location = getRandomCorridor();
 	switch(chance)
	{
	case 0:
		spawnEnemy(
			location.y, 
			location.x, 
			std::make_shared<Dragon>(
				*this, 
				location.x, 
				location.y, 
				static_cast<int>(rng_.nextInt(1000, 1500)*Game::getInstance().score_factor()),
				static_cast<int>(30*Game::getInstance().exp_factor()))
			);
		break;
	case 1:
		spawnEnemy(
			location.y, 
			location.x, 
			std::make_shared<Goblin>(
				*this, 
				location.x, 
				location.y, 
				static_cast<int>(rng_.nextInt(1000, 1500)*Game::getInstance().score_factor()),
				static_cast<int>(30*Game::getInstance().exp_factor()))
			);
		break;
	case 2:
  		spawnEnemy(
			location.y, 
			location.x, 
			std::make_shared<Skeleton>(
				*this, 
				location.x, 
				location.y,
				static_cast<int>(rng_.nextInt(1000, 1500)*Game::getInstance().score_factor()),
				static_cast<int>(30*Game::getInstance().exp_factor()))
			);
		break;
	case 3:
		spawnEnemy(
			location.y, 
			location.x, 
			std::make_shared<Troll>(
				*this, 
				location.x, 
				location.y,
				static_cast<int>(rng_.nextInt(1000, 1500)*Game::getInstance().score_factor()),
				static_cast<int>(30*Game::getInstance().exp_factor()))
			);
		break;
	}

	// Generate a map item of the dungeon
	location = getRandomLit();
	map_.at(location.y, location.x).add(std::make_shared<MapItem>());

	// Generate a compass item of the dungeon
	location = getRandomLit();
	map_.at(location.y, location.x).add(std::make_shared<CompassItem>());

	// Generate the downstairs
	stairs_location_ = location = getRandomLit();
	map_.at(location.y, location.x).add(std::make_shared<Stairs>());

	// Count the number of not explored tiles
	for(int i = map_error; i <= map_.height()-map_error; ++i)
	{
		for(int j = map_error; j <= map_.width()-map_error; ++j)
		{
			if(map_.at(i, j).top()->type() != GameObject::Type::None)
			{
				map_.increase_valid_objects();
			}
		}
	}

	// Empower the enemies
	if(floor_ > 1)
	{
		for(auto enemy : enemies_)
		{
			enemy->levelUp();
		}
	}
}

Map2D& Dungeon::map()
{
	return map_;
}

bool Dungeon::makeSquaredRoom(Point &loc, int height, int width)
{
	int real_height = rng_.nextInt(min_room_height, height);
	int real_width = rng_.nextInt(min_room_width, width);

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
				   (map_.at(i, j).top()->type() != GameObject::Type::None))
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
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(i == loc.y)
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else if(i == (loc.y-real_height+1))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else 
				{
					map_.at(i, j).add(std::make_shared<Lit>());
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
				   (map_.at(i, j).top()->type() != GameObject::Type::None))
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
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(j == (loc.x+real_width-1))
				{
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else 
				{
					map_.at(i, j).add(std::make_shared<Lit>());
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
				   (map_.at(i, j).top()->type() != GameObject::Type::None)) 
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
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(j == (loc.x+(real_width-1)/2))
				{
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(i == loc.y)
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else if(i == (loc.y+real_height-1))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else 
				{
					map_.at(i, j).add(std::make_shared<Lit>());
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
				   (map_.at(i, j).top()->type() != GameObject::Type::None))
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
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(j == (loc.x-real_width+1))
				{
					map_.at(i, j).add(std::make_shared<VerticalWall>());
				}
				else if(i == (loc.y-real_height/2))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else if(i == (loc.y+(real_height-1)/2))
				{
					map_.at(i, j).add(std::make_shared<HorizontalWall>());
				}
				else 
				{
					map_.at(i, j).add(std::make_shared<Lit>());
				}
			}
		}
		break;
	}

	// Let's spawn some objects in the map
	int chance = rng_.nextInt(0, 100);
	if(chance < 20) // 20%
	{
		// Spawn a chest
		// The 2 means two tiles away walls
		spawn(loc, 
			std::make_shared<Chest>(
				static_cast<int>(rng_.nextInt(100, 1000)*Game::getInstance().score_factor())
			), 
			2);
	}
	else if(chance < 100) // 80% <- (100-20)
	{
		// Spawn an enemy
		int monster_chance = rng_.nextInt(0, 3);
		switch(monster_chance)
		{
		case 0:
			spawnEnemy(loc, 
				std::make_shared<SmallGoblin>(
					*this, 
					loc.x, 
					loc.y, 
					static_cast<int>(rng_.nextInt(500, 1000)*Game::getInstance().score_factor()),
					static_cast<int>(15*Game::getInstance().exp_factor())
				), 
				1);
			break;
		case 1:
			spawnEnemy(
				loc, 
				std::make_shared<SmallDragon>(
					*this, 
					loc.x, 
					loc.y,
					static_cast<int>(rng_.nextInt(500, 1000)*Game::getInstance().score_factor()),
					static_cast<int>(15*Game::getInstance().exp_factor())
				),
				1);
			break;
		case 2:
			spawnEnemy(
				loc, 
				std::make_shared<SmallSkeleton>(
					*this, 
					loc.x, 
					loc.y,
					static_cast<int>(rng_.nextInt(500, 1000)*Game::getInstance().score_factor()),
					static_cast<int>(15*Game::getInstance().exp_factor())
				),
				1);
			break;
		case 3:
			spawnEnemy(
				loc, 
				std::make_shared<SmallTroll>(
					*this, 
					loc.x, 
					loc.y,
					static_cast<int>(rng_.nextInt(500, 1000)*Game::getInstance().score_factor()),
					static_cast<int>(15*Game::getInstance().exp_factor())
				), 
				1);
			break;
		}

		++num_enemies_;
	}
	// Otherwise don't spawn nothing

	++num_rooms_;

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
			   (map_.at(i, loc.x).top()->type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		// All went ok? Then build the corridor
		for(int i = loc.y; i > (loc.y-length); --i)
		{
			map_.at(i, loc.x).add(std::make_shared<Corridor>());
		}
		break;
	case Direction::East:
		for(int i = loc.x; i < (loc.x+length); ++i)
		{
			if((i < map_error) || 
			   (i > map_.width()-map_error) ||
			   (map_.at(loc.y, i).top()->type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.x; i < (loc.x+length); ++i)
		{
			map_.at(loc.y, i).add(std::make_shared<Corridor>());
		}
		break;
	case Direction::South:
		for(int i = loc.y; i < (loc.y+length); ++i)
		{
			if((i < map_error) || 
			   (i > map_.height()-map_error) ||
			   (map_.at(i, loc.x).top()->type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.y; i < (loc.y+length); ++i)
		{
			map_.at(i, loc.x).add(std::make_shared<Corridor>());
		}
		break;
	case Direction::West:
		for(int i = loc.x; i > (loc.x-length); --i)
		{
			if((i < map_error) || 
			   (i > map_.width()-map_error) ||
			   (map_.at(loc.y, i).top()->type() != GameObject::Type::None)) 
			{
				return false;
			}
		}

		for(int i = loc.x; i > (loc.x-length); --i)
		{
			map_.at(loc.y, i).add(std::make_shared<Corridor>());
		}
		break;
	}

	++num_corridors_;

	return true;
}

Point Dungeon::getRandomWall()
{
	Point p;

	bool ok = false;
	while(!ok) 
	{
		p.y = rng_.nextInt(2, map_.height()-2);
		p.x = rng_.nextInt(2, map_.width()-2);

		GameObject::Type game_object_type = map_.at(p.y, p.x).top()->type();

		if((game_object_type == GameObject::Type::HorizontalWall) ||
		   (game_object_type == GameObject::Type::VerticalWall) ||
		   (game_object_type == GameObject::Type::Corridor))
		{
			// Check if we can reach the place
			if(map_.at(p.y+1, p.x).top()->walkable())
			{
				p.dir = Direction::North;
				p.x_mod = 0;
				p.y_mod = -1;
			}
			else if(map_.at(p.y, p.x-1).top()->walkable())
			{
				p.dir = Direction::East;
				p.x_mod = +1;
				p.y_mod = 0;
			}
			else if(map_.at(p.y-1, p.x).top()->walkable())
			{
				p.dir = Direction::South;
				p.x_mod = 0;
				p.y_mod = +1;
			}
			else if(map_.at(p.y, p.x+1).top()->walkable())
			{
				p.dir = Direction::West;
				p.x_mod = -1;
				p.y_mod = 0;
			}

			// Check for nearby doors
			if(p.dir != Direction::None)
			{
				if(!checkObjectsSurrounding(p, GameObject::Type::Door, 1))
				{
					ok = true;
				}
			}

			if(!checkCorners(p))
			{
				ok = true;
				p.x = -900;
			}
		}
	}

	return p;
}

Point Dungeon::getRandomLit()
{
	Point p;

	bool ok = false;
	while(!ok) 
	{
		p.y = rng_.nextInt(2, map_.height()-2);
		p.x = rng_.nextInt(2, map_.width()-2);

		if(map_.at(p.y, p.x).top()->type() == GameObject::Type::Lit)
		{
			ok = true;
		}
	}

	return p;
}

Point Dungeon::getRandomCorridor()
{
	Point p;

	bool ok = false;
	while(!ok) 
	{
		p.y = rng_.nextInt(2, map_.height()-2);
		p.x = rng_.nextInt(2, map_.width()-2);

		if(map_.at(p.y, p.x).top()->type() == GameObject::Type::Corridor)
		{
			ok = true;
		}
	}

	return p;
}

bool Dungeon::spawn(int row, int column, GameObjectPtr game_object)
{
	Tile &t = map_.at(row, column);
	if(t.top()->walkable())
	{
		t.add(game_object);
		
		return true;
	}
	
	return false;
}

bool Dungeon::spawnEnemy(int row, int column, GameObjectPtr game_object)
{
	Tile &t = map_.at(row, column);
	if(t.top()->walkable())
	{
		EnemyPtr eptr = std::dynamic_pointer_cast<Enemy>(game_object);
		eptr->location().x = column;
		eptr->location().y = row;
		enemies_.push_back(eptr);

		return true;
	}

	return false;
}

bool Dungeon::spawn(Point &p, GameObjectPtr game_object, int offset)
{
	int real_x = 0;
	int real_y = 0;

	switch(p.dir)
	{
		case Direction::North:
			real_x = p.x;
			real_y = p.y-min_room_height+offset;
			break;
		case Direction::East:
			real_x = p.x+min_room_width-offset;
			real_y = p.y;
			break;
		case Direction::South:
			real_x = p.x;
			real_y = p.y+min_room_height-offset;
			break;
		case Direction::West:
			real_x = p.x-min_room_width+offset;
			real_y = p.y;
			break;
	}

	Tile &t = map_.at(real_y, real_x);

	if(t.top()->walkable())
	{
		t.add(game_object);
		
		return true;
	}

	return false;
}

bool Dungeon::spawnEnemy(Point &p, EnemyPtr game_object, int offset)
{
	int real_x = 0;
	int real_y = 0;

	switch(p.dir)
	{
	case Direction::North:
		real_x = p.x;
		real_y = p.y-min_room_height+offset;
		break;
	case Direction::East:
		real_x = p.x+min_room_width-offset;
		real_y = p.y;
		break;
	case Direction::South:
		real_x = p.x;
		real_y = p.y+min_room_height-offset;
		break;
	case Direction::West:
		real_x = p.x-min_room_width+offset;
		real_y = p.y;
		break;
	}

	Tile &t = map_.at(real_y, real_x);

	if(t.top()->walkable())
	{
		EnemyPtr eptr = std::dynamic_pointer_cast<Enemy>(game_object);
		eptr->location().x = real_x;
		eptr->location().y = real_y;
		enemies_.push_back(eptr);

		return true;
	}

	return false;
}

bool Dungeon::checkObjectsSurrounding(Point &loc, GameObject::Type type, int radius)
{
	for(int i = 0; i <= radius; ++i)
	{
		if((map_.at(loc.y-i, loc.x).top()->type() == type) || // North
			(map_.at(loc.y, loc.x+i).top()->type() == type) || // East
			(map_.at(loc.y+i, loc.x).top()->type() == type) || // South
			(map_.at(loc.y, loc.x-i).top()->type() == type) || // West
			(map_.at(loc.y-i, loc.x+i).top()->type() == type) || // North-East
			(map_.at(loc.y+i, loc.x+i).top()->type() == type) || // South-East
			(map_.at(loc.y+i, loc.x-i).top()->type() == type) || // South-West
			(map_.at(loc.y-i, loc.x-i).top()->type() == type))   // North-West
		{
			return true;
		}
	}

	return false;
}

bool Dungeon::checkCorners(Point &loc)
{
	if(map_.at(loc.y, loc.x).top()->type() == GameObject::Type::VerticalWall)
	{
		// Upper-left corner
		if((map_.at(loc.y+1, loc.x).top()->type() == GameObject::Type::VerticalWall) &&
		   (map_.at(loc.y, loc.x+1).top()->type() == GameObject::Type::HorizontalWall))
		{
			return false;
		}

		// Upper-right corner
		if((map_.at(loc.y+1, loc.x).top()->type() == GameObject::Type::VerticalWall) &&
			(map_.at(loc.y, loc.x-1).top()->type() == GameObject::Type::HorizontalWall))
		{
			return false;
		}

		// Bottom-right corner
		if((map_.at(loc.y-1, loc.x).top()->type() == GameObject::Type::VerticalWall) &&
			(map_.at(loc.y, loc.x-1).top()->type() == GameObject::Type::HorizontalWall))
		{
			return false;
		}

		// Bottom-left corner
		if((map_.at(loc.y-1, loc.x).top()->type() == GameObject::Type::VerticalWall) &&
			(map_.at(loc.y, loc.x+1).top()->type() == GameObject::Type::HorizontalWall))
		{
			return false;
		}
	}

	return true;
}

void Dungeon::draw(WINDOW *win)
{
	int height = map_.height();
	int width = map_.width();

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			GameObjectPtr g = map_.at(i, j).top();

			if(!g->discovered())
			{
				if(Game::getInstance().view_map())
				{
					Tile &t = map_.at(i, j);
					if(t.elements() > 1)
					{
						Curses::mvwaddch(win, i, j, 
							static_cast<char>(t.element(1)->type()) | 
							COLOR_PAIR(static_cast<int>(GameObject::Color::White_Black)));
					}

					if(g->type() == GameObject::Type::DownStairs)
					{
						Curses::mvwaddch(win, i, j, 
							static_cast<char>(g->type()) | 
							COLOR_PAIR(static_cast<int>(g->color())));
					}
				}

				if(Game::getInstance().view_chests())
				{
					if(g->type() == GameObject::Type::Chest)
					{
						Curses::mvwaddch(win, i, j, 
							static_cast<char>(g->type()) | 
							COLOR_PAIR(static_cast<int>(g->color())));
					}
					else if(g->type() == GameObject::Type::DownStairs)
					{
						Curses::mvwaddch(win, i, j, 
							static_cast<char>(g->type()) | 
							COLOR_PAIR(static_cast<int>(g->color())));
					}
				}
			}
			else
			{
				Curses::mvwaddch(win, i, j, 
					static_cast<char>(g->type()) | 
					COLOR_PAIR(static_cast<int>(g->color())));
			}
		}
	}
}

int Dungeon::num_rooms() const
{
	return num_rooms_;
}

int Dungeon::num_corridors() const
{
	return num_corridors_;
}

int Dungeon::num_enemies() const
{
	return num_enemies_;
}

int Dungeon::floor() const
{
	return floor_;
}

void Dungeon::reset_floor()
{
	floor_ = 0;
}

const Point& Dungeon::stairs_location() const
{
	return stairs_location_;
}

std::vector<EnemyPtr>& Dungeon::enemies()
{
	return enemies_;
}

void Dungeon::restart()
{
	map_.clear();
	if(!enemies_.empty())
	{
		enemies_.clear();
	}

	num_corridors_ = 0;
	num_enemies_ = 0;
	num_rooms_ = 0;
}

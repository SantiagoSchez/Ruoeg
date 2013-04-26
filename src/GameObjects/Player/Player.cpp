#include "Player.h"
#include "../Terrains/Door/Door.h"
#include "../../Game/Game.h"
#include "../../Game/ResourceManager.h"

Player::Player(Race race, Map2D &map) : race_(race), 
	GameObject(GameObject::Type::Player), experience_points_(0), level_(1),
	map_(map)
{
	walkable_ = true;
	location_.dir = Dungeon::Direction::North;
}

Player::~Player()
{
}

Player::Race Player::race() const
{
	return race_;
}

int Player::health_points() const
{
	return health_points_;
}

int Player::attack_points() const
{
	return attack_points_;
}

int Player::armor_points() const
{
	return armor_points_;
}

int Player::experience_points() const
{
	return experience_points_;
}

int Player::level() const
{
	return level_;
}

bool Player::isAlive() const
{
	return health_points_ > 0;
}

const Dungeon::Point& Player::location() const
{
	return location_;
}

void Player::placeIt(int x, int y)
{
	location_.x = x;
	location_.y = y;
}

bool Player::moveNorth()
{
	GameObject &game_object = map_.at(location_.y-1, location_.x).top();
	if(game_object.walkable())
	{
		location_.y -= 1;

		return true;
	}

	checkCollisions(game_object);

	return false;
}

bool Player::moveEast()
{
	GameObject &game_object = map_.at(location_.y, location_.x+1).top();
	if(game_object.walkable())
	{
		location_.x += 1;

		return true;
	}
	
	checkCollisions(game_object);

	return false;
}

bool Player::moveSouth()
{
	GameObject &game_object = map_.at(location_.y+1, location_.x).top();
	if(game_object.walkable())
	{
		location_.y += 1;

		return true;
	}

	checkCollisions(game_object);

	return false;
}

bool Player::moveWest()
{
	GameObject &game_object = map_.at(location_.y, location_.x-1).top();
	if(game_object.walkable())
	{
		location_.x -= 1;

		return true;
	}

	checkCollisions(game_object);

	return false;
}

void Player::checkCollisions(GameObject &game_object)
{
	switch(game_object.type())
	{
	case GameObject::Type::Door:
		reinterpret_cast<Door&>(game_object).open();
		Curses::waddstr(Game::getInstance().consoleWindow(), ResourceManager::getInstance().getString("DOOR_OPENED"));
		break;
	}
}

void Player::doFOV()
{
	float x, y, ox, oy;

	for(int i = 0; i < 360; ++i)
	{
		x = cos(static_cast<float>(i*0.01745f));
		y = sin(static_cast<float>(i*0.01745f));

		ox = static_cast<float>(location_.x + 0.5f);
		oy = static_cast<float>(location_.y + 0.5f);
		
		for(int j = 0; j < 4; ++j)
		{
			GameObject &g = map_.at(static_cast<int>(oy), 
				static_cast<int>(ox)).top();
			g.set_in_fov(true);

			if((g.type() == GameObject::Type::HorizontalWall) ||
			   (g.type() == GameObject::Type::VerticalWall) ||
			   (g.type() == GameObject::Type::None) ||
			   (g.type() == GameObject::Type::Door))
			{
				break;
			}

			ox += x;
			oy += y;
		}
	}
}

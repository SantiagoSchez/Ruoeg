#include "Player.h"
#include "../Terrains/Door/Door.h"
#include "../../Game/Game.h"

Player::Player(Race race) : race_(race), 
	GameObject(GameObject::Type::Player), experience_points_(0), level_(1)
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

void Player::placeIt(int x, int y)
{
	location_.x = x;
	location_.y = y;
}

bool Player::moveNorth(Map2D &map)
{
	GameObject &game_object = map.at(location_.y-1, location_.x).top();
	if(game_object.walkable())
	{
		location_.y -= 1;

		return true;
	}

	checkCollisions(game_object);

	return false;
}

bool Player::moveEast(Map2D &map)
{
	GameObject &game_object = map.at(location_.y, location_.x+1).top();
	if(game_object.walkable())
	{
		location_.x += 1;

		return true;
	}
	
	checkCollisions(game_object);

	return false;
}

bool Player::moveSouth(Map2D &map)
{
	GameObject &game_object = map.at(location_.y+1, location_.x).top();
	if(game_object.walkable())
	{
		location_.y += 1;

		return true;
	}

	checkCollisions(game_object);

	return false;
}

bool Player::moveWest(Map2D &map)
{
	GameObject &game_object = map.at(location_.y, location_.x-1).top();
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
	if(game_object.type() == GameObject::Type::Door)
	{
		reinterpret_cast<Door&>(game_object).open();
		Curses::waddstr(Game::getInstance().console(), " You open the door!\n");
	}
}
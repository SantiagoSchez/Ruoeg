#include <iostream>
#include <typeinfo>

#include "Player.h"
#include "../Chests/Chest.h"
#include "../Terrains/Door/Door.h"
#include "../Enemies/SmallGoblin/SmallGoblin.h"
#include "../Terrains/Lit/Lit.h"
#include "../../Game/Game.h"
#include "../../Game/ResourceManager.h"

Player::Player(Race race, Map2D &map) : race_(race), 
	GameObject(GameObject::Type::Player), experience_points_(0), 
	max_experience_points_(50), level_(1), map_(map), explored_(0)
{
	switch(race)
	{
	case Race::Human:
		str_race_ = "Human";
		break;
	case Race::Orc:
		str_race_ = "Orc";
		break;
	case Race::Elf:
		str_race_ = "Elf";
		break;
	case Race::Dwarf:
		str_race_ = "Dwarf";
		break;
	}

	in_fov_ = true;
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

const char* Player::str_race() const
{
	return str_race_;
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

int Player::max_experience_points() const
{
	return max_experience_points_;
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
	GameObjectPtr game_object = map_.at(location_.y-1, location_.x).top();
	if(game_object->walkable())
	{
		location_.y -= 1;

		return true;
	}

	checkCollisions(std::move(game_object));

	return false;
}

bool Player::moveEast()
{
	GameObjectPtr game_object = map_.at(location_.y, location_.x+1).top();
	if(game_object->walkable())
	{
		location_.x += 1;

		return true;
	}
	
	checkCollisions(std::move(game_object));

	return false;
}

bool Player::moveSouth()
{
	GameObjectPtr game_object = map_.at(location_.y+1, location_.x).top();
	if(game_object->walkable())
	{
		location_.y += 1;

		return true;
	}

	checkCollisions(std::move(game_object));

	return false;
}

bool Player::moveWest()
{
	GameObjectPtr game_object = map_.at(location_.y, location_.x-1).top();
	if(game_object->walkable())
	{
		location_.x -= 1;

		return true;
	}

	checkCollisions(std::move(game_object));

	return false;
}

void Player::checkCollisions(GameObjectPtr game_object)
{
	switch(game_object->type())
	{
	case GameObject::Type::Door:
		dynamic_cast<Door&>(*game_object).open();
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s\n", 
			ResourceManager::getInstance().getString("DOOR_OPENED"));
		break;
	case GameObject::Type::Chest:
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d%s\n", 
			ResourceManager::getInstance().getString("CHEST_GATHERED1"),
			Game::getInstance().current_score_value(),
			ResourceManager::getInstance().getString("CHEST_GATHERED2"));
		Game::getInstance().add_score(1);
		// Remove chest
		game_object->set_in_fov(true);
		Chest::decrease_num_chests();
		break;
	case GameObject::Type::Dragon:
	case GameObject::Type::Goblin:
	case GameObject::Type::Skeleton:
	case GameObject::Type::Troll:
	case GameObject::Type::SmallDragon:
	case GameObject::Type::SmallGoblin:
	case GameObject::Type::SmallSkeleton:
	case GameObject::Type::SmallTroll:
		attack(game_object);
		break;
	}
}

void Player::attack(GameObjectPtr game_object)
{
	Enemy &enemy = dynamic_cast<Enemy&>(*game_object);
	int damage = enemy.receiveDamage(attack_points_);

	// Show enemy level and life
	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d | [%d/%d]",
		ResourceManager::getInstance().getString("ENEMY_LEVEL"),
		enemy.level(),
		enemy.health(),
		enemy.max_health());
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));

	// Show enemy attack sentence
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s ", 
		ResourceManager::getInstance().getString("ATTACK_ENEMY1"));
	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), "%s ",
		enemy.name());
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));
	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Red_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), "(%s %d)\n",
		ResourceManager::getInstance().getString("ATTACK_ENEMY2"),
		damage);
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Red_Black));
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
			GameObjectPtr g = map_.at(static_cast<int>(oy), 
				static_cast<int>(ox)).top();
			if(!g->in_fov())
			{
				g->set_in_fov(true);
				if(g->type() != GameObject::Type::None)
				{
					++explored_;
					Game::getInstance().add_score(0.01);
				}
			}

			if((g->type() == GameObject::Type::HorizontalWall) ||
				(g->type() == GameObject::Type::VerticalWall) ||
				(g->type() == GameObject::Type::None) ||
				(g->type() == GameObject::Type::Door))
			{
				break;
			}

			ox += x;
			oy += y;
		}
	}
}

int Player::explored() const
{
	return explored_;
}

void Player::reset_explored()
{
	explored_ = 0;
}
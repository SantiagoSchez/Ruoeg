#include <iostream>
#include <typeinfo>

#include "Player.h"
#include "../Enemies/Enemy.h"
#include "../Chests/Chest.h"
#include "../Terrains/Door/Door.h"
#include "../Enemies/SmallGoblin/SmallGoblin.h"
#include "../Terrains/Lit/Lit.h"
#include "../../Game/Game.h"
#include "../../Game/ResourceManager.h"

Player::Player(Race race, Dungeon &dungeon) : race_(race), 
	GameObject(GameObject::Type::Player), experience_points_(0), 
	max_experience_points_(50), level_(1), dungeon_(dungeon), explored_(0),
	ghost_mode(false)
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

	discovered_ = true;
	walkable_ = true;
	location_.dir = Direction::North;
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

int Player::max_health_points() const
{
	return max_health_points_;
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

const Point& Player::location() const
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
	// DEBUG
	if(ghost_mode)
	{
		location_.y -= 1;
		return true;
	}

	for(auto enemy : dungeon_.enemies())
	{
		if(enemy->location().y == location_.y-1)
		{
			if(enemy->location().x == location_.x)
			{
				attack(*enemy);
				return false;
			}
		}
	}

	Tile &t = dungeon_.map().at(location_.y-1, location_.x);
	if(t.top()->walkable())
	{
		location_.y -= 1;
		return true;
	}

	checkMapCollisions(t);

	return false;
}

bool Player::moveEast()
{
	// DEBUG
	if(ghost_mode)
	{
		location_.x += 1;
		return true;
	}

	for(auto enemy : dungeon_.enemies())
	{
		if(enemy->location().x == location_.x+1)
		{
			if(enemy->location().y == location_.y)
			{
				attack(*enemy);
				return false;
			}
		}
	}

	Tile &t = dungeon_.map().at(location_.y, location_.x+1);
	if(t.top()->walkable())
	{
		location_.x += 1;
		return true;
	}
	
	checkMapCollisions(t);

	return false;
}

bool Player::moveSouth()
{
	// DEBUG
	if(ghost_mode)
	{
		location_.y += 1;
		return true;
	}

	for(auto enemy : dungeon_.enemies())
	{
		if(enemy->location().y == location_.y+1)
		{
			if(enemy->location().x == location_.x)
			{
				attack(*enemy);
				return false;
			}
		}
	}

	Tile &t = dungeon_.map().at(location_.y+1, location_.x);
	if(t.top()->walkable())
	{
		location_.y += 1;
		return true;
	}

	checkMapCollisions(t);

	return false;
}

bool Player::moveWest()
{
	// DEBUG
	if(ghost_mode)
	{
		location_.x -= 1;
		return true;
	}

	for(auto enemy : dungeon_.enemies())
	{
		if(enemy->location().x == location_.x-1)
		{
			if(enemy->location().y == location_.y)
			{
				attack(*enemy);
				return false;
			}
		}
	}

	Tile &t = dungeon_.map().at(location_.y, location_.x-1);
	if(t.top()->walkable())
	{
		location_.x -= 1;
		return true;
	}

	checkMapCollisions(t);

	return false;
}

void Player::checkMapCollisions(Tile &tile)
{
	GameObjectPtr game_object = tile.top();

	switch(game_object->type())
	{
	case GameObject::Type::Door:
		openDoor(static_cast<Door&>(*game_object));
		break;
	case GameObject::Type::Chest:
		openChest(static_cast<Chest&>(*game_object));
		break;
	case GameObject::Type::DownStairs:
		if(!goDeep())
		{
			location_ = dungeon_.stairs_location();
			Game::getInstance().increase_factors();
		}
		break;
	}
}

void Player::openDoor(Door &door)
{
	door.open();
	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Cyan_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s\n", 
		ResourceManager::getInstance().getString("DOOR_OPENED"));
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Cyan_Black));
}

void Player::openChest(Chest &chest)
{
	int score = chest.score();

	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Green_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %d %s\n", 
		score,
		ResourceManager::getInstance().getString("CHEST_GATHERED"));
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Green_Black));

	Game::getInstance().add_score(score);
	chest.kill_object();
}

void Player::attack(Enemy &enemy)
{
	int damage = enemy.receiveDamage(*this);

	if(enemy.health() <= 0)
	{
		return;
	}

	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s\n",
		ResourceManager::getInstance().getString("ATTACK_ENEMY1"));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s",
		ResourceManager::getInstance().getString("ENEMY_LEVEL"));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s",
		enemy.toString());
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Yellow_Black));
	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Green_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " (%s %d)\n",
		ResourceManager::getInstance().getString("ATTACK_ENEMY2"),
		damage);
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Green_Black));
}

void Player::doFOV()
{
	float x, y, ox, oy;

	for(int i = 0; i < 360; i += 4)
	{
		x = cos(static_cast<float>(i*0.01745f));
		y = sin(static_cast<float>(i*0.01745f));

		ox = static_cast<float>(location_.x + 0.5f);
		oy = static_cast<float>(location_.y + 0.5f);

		for(int j = 0; j < 4; ++j)
		{
			// Check for static objects
			GameObjectPtr g = dungeon_.map().at(static_cast<int>(oy), 
				static_cast<int>(ox)).top();

			if(!g->discovered())
			{
				g->set_discovered(true);

				if(g->type() != GameObject::Type::None)
				{
					++explored_;
					Game::getInstance().add_score(1);
				}
			}

			// Check for enemies
			for(auto enemy : dungeon_.enemies())
			{
				if(enemy->location().x == static_cast<int>(ox))
				{
					if(enemy->location().y == static_cast<int>(oy))
					{
						enemy->set_discovered(true);
					}
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

int Player::receiveDamage(Enemy &enemy)
{
	color_ = GameObject::Color::Red_Black;

	int damage = rng_.nextInt(enemy.attack()-2, enemy.attack()+2) - armor_points_;
	if(damage <= 0)
	{
		damage = 1;
	}
	health_points_ -= damage;

	Curses::wattron(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Red_Black));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s %s %s\n",
		ResourceManager::getInstance().getString("ATTACK_PLAYER1"),
		enemy.name(),
		ResourceManager::getInstance().getString("ATTACK_PLAYER2"));
	Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d %s (%d/%d)\n", 
		ResourceManager::getInstance().getString("YOU_RECEIVE"),
		damage,
		ResourceManager::getInstance().getString("DAMAGE_RECEIVED"),
		health_points_,
		max_health_points_);
	Curses::wattroff(Game::getInstance().consoleWindow(), 
		COLOR_PAIR(GameObject::Color::Red_Black));

	if(health_points_ <= 0)
	{
		// Player is dead
		health_points_ = 0;
	}

	return damage;
}

void Player::addExp(int exp)
{
	experience_points_ += exp;

	if(experience_points_ >= max_experience_points_)
	{
		levelUp();

		Curses::wattron(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Green_Black));
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d\n", 
			ResourceManager::getInstance().getString("LVL_UP"),
			level_);
		Curses::wattroff(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Green_Black));
	}
}

bool Player::goDeep()
{
	WINDOW *prompt = Curses::derwin(Game::getInstance().mapWindow(), 
		5, 35, dungeon_.map().height()/2-2, dungeon_.map().width()/2-17);

	Curses::wbkgd(prompt, COLOR_PAIR(GameObject::Color::Yellow_Black));
	
	while(prompt != NULL)
	{
		if(prompt != NULL)
		{
			Curses::mvwprintw(prompt, 1, 1, "You will go deeper in the dungeon.");
			Curses::mvwprintw(prompt, 2, 1, "                                  ");
			Curses::mvwprintw(prompt, 3, 1, "           [Y]es | [N]o           ");
			Curses::mvwprintw(prompt, 4, 1, "                                  ");

			Curses::wbox(prompt, 0, 0);
			Curses::wrefresh(prompt);
		}

		int key = Curses::wgetch(prompt);
		if(key != -1)
		{
			if(key == static_cast<int>('y'))
			{
				explored_ = 0;
				dungeon_.generate();
				Curses::delwin(prompt);

				return true;
			}
			else if(key == static_cast<int>('n'))
			{
				Curses::delwin(prompt);
				Curses::werase(Game::getInstance().mapWindow());
				return false;
			}
		}
	}

	return false;
}

#include "Enemy.h"
#include "../Player/Player.h"
#include "../../Game/Game.h"
#include "../../Game/ResourceManager.h"

int Enemy::num_enemies_;

Enemy::Enemy(Type type, Dungeon &dungeon, int x, int y, int score, int exp) : 
	GameObject(type), location_(x, y), name_("!"), level_(-1), health_(-1), 
	max_health_(-1), attack_(-1), armor_(-1), dungeon_(dungeon), score_(score),
	exp_(exp)
{
	walkable_ = false;
	++num_enemies_;
}

Enemy::~Enemy()
{
	--num_enemies_;
}

const char* Enemy::name() const
{
	return name_;
}

int Enemy::level() const
{
	return level_;
}

int Enemy::health() const
{
	return health_;
}

int Enemy::max_health() const
{
	return max_health_;
}

int Enemy::attack() const
{
	return attack_;
}

int Enemy::armor() const
{
	return armor_;
}

int Enemy::receiveDamage(Player &player)
{
	color_ = GameObject::Color::Red_Black;

	int damage = rng_.nextInt(player.attack_points()-2, player.attack_points()+2) - armor_;
	if(damage <= 0)
	{
		damage = 1;
	}
	health_ -= damage;

	if(health_ <= 0)
	{
		delete_object_ = true;
		health_ = 0;

		Curses::wattron(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Yellow_Black));
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s %s!\n", 
			ResourceManager::getInstance().getString("YOU_KILLED"),
			name_);
		Curses::wattroff(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Yellow_Black));

		Curses::wattron(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Green_Black));
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d %s\n", 
			ResourceManager::getInstance().getString("YOU_RECEIVE"),
			score_,
			ResourceManager::getInstance().getString("GOLD_RECEIVED"));
		Curses::wattroff(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Green_Black));

		Curses::wattron(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Magenta_Black));
		Curses::wprintw(Game::getInstance().consoleWindow(), " %s %d %s\n", 
			ResourceManager::getInstance().getString("YOU_RECEIVE"),
			exp_,
			ResourceManager::getInstance().getString("EXP_RECEIVED"));
		Curses::wattroff(Game::getInstance().consoleWindow(), 
			COLOR_PAIR(GameObject::Color::Magenta_Black));

		Game::getInstance().add_score(score_);
		player.addExp(exp_);
	}

	return damage;
}

const char* Enemy::toString()
{
	string_ = std::to_string(level_) + " | [" +
		std::to_string(health_) + "/" + std::to_string(max_health_) +
		"] | " + name_;

	return string_.c_str();
}

Point& Enemy::location()
{
	return location_;
}

int Enemy::num_enemies()
{
	return num_enemies_;
}

void Enemy::draw(WINDOW *win)
{
	if(discovered_)
	{
		Curses::mvwaddch(win, location_.y, location_.x, 
			static_cast<char>(type_) | 
			COLOR_PAIR(static_cast<int>(color_)));

		if(color_ != GameObject::Color::Yellow_Black)
		{
			wnoutrefresh(win);
			Curses::napms(50);
			color_ = GameObject::Color::Yellow_Black;
		}
	}
}

void Enemy::update(Player &player)
{
	if(health_ > 0)
	{
		int px = player.location().x;
		int py = player.location().y;
		int &ex = location_.x;
		int &ey = location_.y;

		// Movement
		if(ey > py)
		{
			moveNorth(player);
		}
		else if(ey < py)
		{
			moveSouth(player);
		}
		if(ex < px)
		{
			moveEast(player);
		}
		else if(ex > px)
		{
			moveWest(player);
		}
	}
}

bool Enemy::moveNorth(Player &player)
{
	if(player.location().x == location_.x)
	{
		if(player.location().y == location_.y-1)
		{
			player.receiveDamage(*this);
			return false;
		}
	}

	Tile &t = dungeon_.map().at(location_.y-1, location_.x);
	if(t.top()->walkable())
	{
		location_.y -= 1;
		return true;
	}

	return false;
}

bool Enemy::moveEast(Player &player)
{
	if(player.location().x == location_.x+1)
	{
		if(player.location().y == location_.y)
		{
			player.receiveDamage(*this);
			return false;
		}
	}

	Tile &t = dungeon_.map().at(location_.y, location_.x+1);
	if(t.top()->walkable())
	{
		location_.x += 1;
		return true;
	}

	return false;
}

bool Enemy::moveSouth(Player &player)
{
	if(player.location().x == location_.x)
	{
		if(player.location().y == location_.y+1)
		{
			player.receiveDamage(*this);
			return false;
		}
	}

	Tile &t = dungeon_.map().at(location_.y+1, location_.x);
	if(t.top()->walkable())
	{
		location_.y += 1;
		return true;
	}

	return false;
}

bool Enemy::moveWest(Player &player)
{
	if(player.location().x == location_.x-1)
	{
		if(player.location().y == location_.y)
		{
			player.receiveDamage(*this);
			return false;
		}
	}

	Tile &t = dungeon_.map().at(location_.y, location_.x-1);
	if(t.top()->walkable())
	{
		location_.x -= 1;
		return true;
	}

	return false;
}

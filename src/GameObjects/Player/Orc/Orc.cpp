#include "Orc.h"

Orc::Orc(Map2D &map) : Player(Player::Race::Orc, map)
{
	health_points_ = 150;
	attack_points_ = 15;
	armor_points_ = 15;
}

Orc::~Orc()
{
}

void Orc::levelUp()
{
	health_points_ += 15;
	attack_points_ += 10;
	armor_points_ += 10;
	level_ += 1;
}

void Orc::draw(WINDOW *win)
{
	doFOV();

	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(GameObject::Color::White_Green)));
}
#include "Orc.h"

Orc::Orc() : Player(Player::Race::Orc)
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
	Curses::wattron(win, COLOR_PAIR(static_cast<int>(GameObject::Color::White_Green)));
	Curses::mvwaddch(win, location_.y, location_.x, static_cast<char>(GameObject::Type::Player));
	Curses::wattroff(win, COLOR_PAIR(static_cast<int>(GameObject::Color::White_Green)));
}
#include "Dwarf.h"

Dwarf::Dwarf() : Player(Player::Race::Dwarf)
{
	health_points_ = 50;
	attack_points_ = 15;
	armor_points_ = 15;
}

Dwarf::~Dwarf()
{
}

void Dwarf::levelUp()
{
	health_points_ += 5;
	attack_points_ += 10;
	armor_points_ += 10;
	level_ += 1;
}

void Dwarf::draw(WINDOW *win)
{
	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(GameObject::Color::White_Red)));
}
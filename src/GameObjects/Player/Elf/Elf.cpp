#include "Elf.h"

Elf::Elf() : Player(Player::Race::Elf)
{
	health_points_ = 50;
	attack_points_ = 5;
	armor_points_ = 5;
}

Elf::~Elf()
{
}

void Elf::levelUp()
{
	health_points_ += 10;
	attack_points_ += 1;
	armor_points_ += 1;
	level_ += 1;
}

void Elf::draw(WINDOW *win)
{
	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(GameObject::Color::White_Yellow)));
}
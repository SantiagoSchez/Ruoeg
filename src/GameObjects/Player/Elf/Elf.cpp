#include "Elf.h"

Elf::Elf(Map2D &map) : Player(Player::Race::Elf, map)
{
	health_points_ = 50;
	attack_points_ = 5;
	armor_points_ = 5;
	color_ = GameObject::Color::White_Yellow;
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
	doFOV();

	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(color_)));

	if(color_ != GameObject::Color::White_Yellow)
	{
		wnoutrefresh(win);
		Curses::napms(50);
		color_ = GameObject::Color::White_Yellow;
	}
}
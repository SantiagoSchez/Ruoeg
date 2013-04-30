#include "Dwarf.h"

Dwarf::Dwarf(Map2D &map) : Player(Player::Race::Dwarf, map)
{
	health_points_ = 50;
	attack_points_ = 15;
	armor_points_ = 15;
	color_ = GameObject::Color::White_Red;
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
	doFOV();

	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(color_)));

	if(color_ != GameObject::Color::White_Red)
	{
		wnoutrefresh(win);
		Curses::napms(50);
		color_ = GameObject::Color::White_Red;
	}
}
#include "Dwarf.h"

Dwarf::Dwarf(Dungeon &dungeon) : Player(Player::Race::Dwarf, dungeon)
{
	max_health_points_ = 30;
	health_points_ = max_health_points_;
	attack_points_ = 10;
	armor_points_ = 10;
	color_ = GameObject::Color::White_Red;
}

Dwarf::~Dwarf()
{
}

void Dwarf::levelUp()
{
	max_health_points_ += 10;
	health_points_ = max_health_points_;
	attack_points_ += 6;
	armor_points_ += 6;
	experience_points_ = 0;
	max_experience_points_ = static_cast<int>(max_experience_points_ * 1.50);
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
		Curses::napms(time_to_flash_);
		color_ = GameObject::Color::White_Red;
	}
}
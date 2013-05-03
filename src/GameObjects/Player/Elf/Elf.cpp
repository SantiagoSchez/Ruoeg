#include "Elf.h"

Elf::Elf(Dungeon &dungeon) : Player(Player::Race::Elf, dungeon)
{
	max_health_points_ = 30;
	health_points_ = max_health_points_;
	attack_points_ = 3;
	armor_points_ = 3;
	color_ = GameObject::Color::White_Yellow;
}

Elf::~Elf()
{
}

void Elf::levelUp()
{
	max_health_points_ += 10;
	health_points_ = max_health_points_;
	attack_points_ += 2;
	armor_points_ += 2;
	experience_points_ = 0;
	max_experience_points_ = static_cast<int>(max_experience_points_ * 1.50);
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
		Curses::napms(time_to_flash_);
		color_ = GameObject::Color::White_Yellow;
	}
}
#include "Human.h"

Human::Human(Map2D &map) : Player(Player::Race::Human, map)
{
	health_points_ = 100;
	attack_points_ = 10;
	armor_points_ = 10;
	color_ = GameObject::Color::White_Black;
}

Human::~Human()
{
}

void Human::levelUp()
{
	health_points_ += 10;
	attack_points_ += 5;
	armor_points_ += 5;
	level_ += 1;
}

void Human::draw(WINDOW *win)
{
	doFOV();

	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(color_)));

	if(color_ != GameObject::Color::White_Black)
	{
		wnoutrefresh(win);
		Curses::napms(50);
		color_ = GameObject::Color::White_Black;
	}
}
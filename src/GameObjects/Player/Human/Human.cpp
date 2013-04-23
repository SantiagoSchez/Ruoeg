#include "Human.h"

Human::Human() : Player(Player::Race::Human)
{
	health_points_ = 100;
	attack_points_ = 10;
	armor_points_ = 10;
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
	Curses::mvwaddch(win, location_.y, location_.x, 
		static_cast<char>(type_) | 
		COLOR_PAIR(static_cast<int>(GameObject::Color::White_Black)));
}
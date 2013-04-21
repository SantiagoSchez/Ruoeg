#include "Game.h"

#include <iostream>

Game::Game() : dungeon_(30, 90)
{
}

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}

void Game::start()
{
	setUp();
	loop();
	release();
}

void Game::setUp()
{
	// Initializing curses stuff
	Crs::initscr();
	Crs::noecho();
	Crs::cbreak();
	Crs::curs_set(0);
	Crs::resize_term(40, 90);

	// Creating the three windows
	windows_.push_back(Crs::newwin(30, 90, 0, 0)); // Map
	windows_.push_back(Crs::newwin(10, 60, 30, 0)); // Console
	windows_.push_back(Crs::newwin(10, 30, 30, 60)); // Status (Life, damage, armor)

	// Input for map window
	Crs::keypad(windows_.at(0), true);
	Crs::nodelay(windows_.at(0), true);

	// Enables scroll for console window
	Crs::scrollok(windows_.at(1), true);

	// Adding a dummy line to bypass the top line in console window
	Crs::waddstr(windows_.at(1), "\n");

	state_ = State::Running;

	if(!Crs::has_colors())
	{	
		Crs::endwin();
		std::cout << "Your terminal does not support color" << std::endl;
		exit(1);
	}

	Crs::start_color();
	Crs::init_pair(1, COLOR_RED, COLOR_BLACK); // Walls/Non-walkable
	Crs::init_pair(2, COLOR_GREEN, COLOR_BLACK); // Floor/Walkable
	Crs::init_pair(3, COLOR_YELLOW, COLOR_RED); // Enemies
	Crs::init_pair(4, COLOR_CYAN, COLOR_BLACK); // Doors/Stairs
	Crs::init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Chests
}

void Game::loop(Crs::Key /*= Crs::Key::ESC*/)
{
	dungeon_.generate();

	while(state_ == State::Running)
	{
		manageInput(windows_.at(0));
		dungeon_.draw(windows_.at(0));

		refreshWindows(windows_);
	}
}

void Game::refreshWindows(std::vector<WINDOW *> windows)
{
	for(auto w : windows)
	{
		Crs::box(w, 0, 0);
		Crs::refresh(w);
	}
}

void Game::release()
{
	Crs::endwin();
}

void Game::manageInput(WINDOW *win)
{
	int key = Crs::wgetch(win);

	if(key != -1)
	{
		// Stuff for testing 
		if(key == static_cast<int>(Crs::Key::Space))
		{
			dungeon_.generate();
		}

		if(key == static_cast<int>(Crs::Key::Left))
		{

		}

		if(key == static_cast<int>(Crs::Key::Right))
		{

		}
	}
}
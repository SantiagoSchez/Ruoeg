#include "Game.h"

#include <iostream>

#include "ResourceManager.h"
#include "../GameObjects/Player/Human/Human.h"
#include "../GameObjects/Player/Orc/Orc.h"
#include "../GameObjects/Player/Elf/Elf.h"
#include "../GameObjects/Player/Dwarf/Dwarf.h"

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
	Curses::initscr();
	Curses::noecho();
	Curses::cbreak();
	Curses::curs_set(0);
	Curses::resize_term(40, 90);

	// Creating the three windows
	windows_.push_back(Curses::newwin(30, 90, 0, 0)); // Map
	windows_.push_back(Curses::newwin(10, 60, 30, 0)); // Console
	windows_.push_back(Curses::newwin(10, 30, 30, 60)); // Status (Life, damage, armor)

	// Input for map window
	Curses::keypad(windows_[0], true);
	Curses::nodelay(windows_[0], true);

	// Enables scroll for console window
	Curses::scrollok(windows_[1], true);

	// Adding a dummy line to bypass the top line in console window
	Curses::waddstr(windows_[1], "\n");

	// Init colors
	if(!Curses::has_colors())
	{	
		Curses::endwin();
		std::cout << "Your terminal does not support color" << std::endl;
		exit(1);
	}

	Curses::start_color();
	Curses::init_pair(static_cast<int>(GameObject::Color::Red_Black), COLOR_RED, COLOR_BLACK);
	Curses::init_pair(static_cast<int>(GameObject::Color::Green_Black), COLOR_GREEN, COLOR_BLACK);
	Curses::init_pair(static_cast<int>(GameObject::Color::Yellow_Red), COLOR_YELLOW, COLOR_RED);
	Curses::init_pair(static_cast<int>(GameObject::Color::Cyan_Black), COLOR_CYAN, COLOR_BLACK);
	Curses::init_pair(static_cast<int>(GameObject::Color::Yellow_Black), COLOR_YELLOW, COLOR_BLACK);
	Curses::init_pair(static_cast<int>(GameObject::Color::White_Black), COLOR_WHITE, COLOR_BLACK);
	Curses::init_pair(static_cast<int>(GameObject::Color::White_Green), COLOR_WHITE, COLOR_GREEN);
	Curses::init_pair(static_cast<int>(GameObject::Color::White_Yellow), COLOR_WHITE, COLOR_YELLOW);
	Curses::init_pair(static_cast<int>(GameObject::Color::White_Red), COLOR_WHITE, COLOR_RED);

	// Load resources
	loadStrings();

	// Init the player
	// TODO: Make a presentation screen that allows the player to choose
	// a race for playing.
	player_.reset(new Orc());
	player_->placeIt(dungeon_.map().width()/2+1, dungeon_.map().height()/2+1);
	//////////////////////////////////////////////////////////////

	// Change game state
	state_ = State::Running;
}

void Game::loop(Curses::Key /*= Crs::Key::ESC*/)
{
	dungeon_.generate();

	while(state_ == State::Running)
	{
		if(manageInput(windows_[0]) != -1)
		{
			// Update here monsters behavior
		}

		dungeon_.draw(windows_[0]);
		player_->draw(windows_[0]);
		
		refreshWindows(windows_);
	}
}

void Game::refreshWindows(std::vector<WINDOW *> windows)
{
	for(auto w : windows)
	{
		Curses::wbox(w, 0, 0);
		Curses::refresh(w);
	}
}

void Game::release()
{
	Curses::endwin();
}

int Game::manageInput(WINDOW *win)
{
	int key = Curses::wgetch(win);

	if(key != -1)
	{
		// Stuff for testing 
		if(key == static_cast<int>(Curses::Key::Space))
		{
			dungeon_.generate();
		}

		// Player movement
		if(key == static_cast<int>(Curses::Key::Up))
		{
			player_->moveNorth(dungeon_.map());
		}
		else if(key == static_cast<int>(Curses::Key::Right))
		{
			player_->moveEast(dungeon_.map());
		}
		else if(key == static_cast<int>(Curses::Key::Down))
		{
			player_->moveSouth(dungeon_.map());
		}
		else if(key == static_cast<int>(Curses::Key::Left))
		{
			player_->moveWest(dungeon_.map());
		}
	}

	return key;
}

WINDOW* Game::mapWindow()
{
	return windows_[0];
}

WINDOW* Game::consoleWindow()
{
	return windows_[1];
}

WINDOW* Game::statusWindow()
{
	return windows_[2];
}

void Game::loadStrings()
{
	ResourceManager::getInstance().addString("DOOR_OPENED",
		" You opened the door.\n");
}
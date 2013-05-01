#include "Game.h"

#include "windows.h"
#include "../../include/ufmod.h"

#include "ResourceManager.h"
#include "../GameObjects/Player/Human/Human.h"
#include "../GameObjects/Player/Orc/Orc.h"
#include "../GameObjects/Player/Elf/Elf.h"
#include "../GameObjects/Player/Dwarf/Dwarf.h"

#include <algorithm>

Game::Game() : dungeon_(30, 90), score_factor_(0.5), view_map_(false)
{
}

Game::~Game()
{
	Curses::endwin();
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
	windows_.push_back(Curses::newwin(10, 30, 30, 60)); // Status

	// Input for map window
	Curses::keypad(windows_[0], true);
	Curses::nodelay(windows_[0], true);

	// Enables scroll for console window
	Curses::scrollok(windows_[1], true);

	// Adding a dummy line to bypass the top line in console window
	Curses::wprintw(windows_[1], "\n");

	// TODO: 
	//  - Make a presentation screen that allows the player to choose
	//    a race for playing.
	//  - Give the user an option for playing with or without colors
	//  - Warn the user playing with colors may results in character
	//    overlapping and so they should change to other not bitmap
	//    font in their console window

	// Init colors
	if(!Curses::has_colors())
	{	
		//std::cout << "Your terminal does not support color" << std::endl;
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
	Curses::init_pair(static_cast<int>(GameObject::Color::Black_Black), COLOR_BLACK, COLOR_BLACK);

	// Load resources
	loadStrings();

	// Init the player and dungeon
	dungeon_.generate();
	Point loc = dungeon_.getRandomLit();
	player_.reset(new Human(dungeon_));
	player_->placeIt(loc.x, loc.y);

	// Change game state
	state_ = State::Running;

	// Play some music
	RNG rng;
	int random_song = rng.nextInt(1, 6);
	uFMOD_PlaySong(reinterpret_cast<char*>(random_song), 0, XM_RESOURCE);
}

void Game::loop(Curses::Key /*= Crs::Key::ESC*/)
{
	while(state_ == State::Running)
	{
		updateEnemyDeaths();

		if(manageInput(windows_[0]) != -1)
		{
			updateEnemies();
		}
		
		updateMapWindow();
		updateStatusWindow();
		
		refreshWindows(windows_);
	}
}

void Game::updateEnemies()
{
	for(auto enemy : dungeon_.enemies())
	{
		if(enemy->discovered())
		{
			enemy->update(*player_);
		}
	}
}

void Game::updateEnemyDeaths()
{
	std::vector<EnemyPtr> &enemies = dungeon_.enemies();

	enemies.erase(
		std::remove_if(
			enemies.begin(),
			enemies.end(),
			[](EnemyPtr enemy) -> bool 
			{
				return enemy->delete_object();
			}
		),
		enemies.end()
	);
}

void Game::refreshWindows(std::vector<WINDOW *> windows)
{
	for(auto w : windows)
	{
		Curses::wbox(w, 0, 0);
		Curses::wnoutrefresh(w);
	}

	Curses::doupdate();
}

int Game::manageInput(WINDOW *win)
{
	int key = Curses::wgetch(win);

	if(key != -1)
	{
		// Stuff for debugging 
		if(key == static_cast<int>(Curses::Key::F1))
		{
			player_->ghost_mode = !player_->ghost_mode;
			Curses::wprintw(windows_[1], " GHOST MODE: %d\n", player_->ghost_mode);
		}
		else if(key == static_cast<int>(Curses::Key::F2))
		{
			view_map_ = !view_map_;
			Curses::werase(windows_[0]);
			Curses::wprintw(windows_[1], " VIEW MAP: %d\n", view_map_);
		}

		// Player movement
		if(key == static_cast<int>(Curses::Key::Up))
		{
			player_->moveNorth();
		}
		else if(key == static_cast<int>(Curses::Key::Right))
		{
			player_->moveEast();
		}
		else if(key == static_cast<int>(Curses::Key::Down))
		{
			player_->moveSouth();
		}
		else if(key == static_cast<int>(Curses::Key::Left))
		{
			player_->moveWest();
		}

		// Exit
		if(key == static_cast<int>(Curses::Key::Esc))
		{
			state_ = State::Finish;
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

void Game::updateMapWindow()
{
	// Draw player and dungeon
	dungeon_.draw(windows_[0]);
	player_->draw(windows_[0]);

	// Draw enemies
	for(auto enemy : dungeon_.enemies())
	{
		enemy->draw(windows_[0]);
	}

	// Show score and map exploration
	Curses::wattron(windows_[0], COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
	Curses::mvwprintw(windows_[0], 1, 1, " %s %d | %s %d", 
		ResourceManager::getInstance().getString("SCORE"), 
		score_,
		ResourceManager::getInstance().getString("HI_SCORE"),
		hi_score_);
 	Curses::mvwprintw(windows_[0], 1, dungeon_.map().width()-27, "%s %.2f%%",
 		ResourceManager::getInstance().getString("EXPLORED"),
 		(player_->explored()/(float)dungeon_.map().valid_objects())*100);
	Curses::wattroff(windows_[0], COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
}

void Game::updateStatusWindow()
{
	// Number of enemies and chests
	Curses::wattron(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));
	Curses::mvwprintw(windows_[2], 1, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("ENEMIES_NUMBER"), 
		Enemy::num_enemies());
	Curses::mvwprintw(windows_[2], 2, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("CHESTS_NUMBER"), 
		Chest::num_chests());
	Curses::wattroff(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));

	// Player level
	Curses::wattron(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
	Curses::mvwprintw(windows_[2], 4, 0, " %s %d %s  ", 
		ResourceManager::getInstance().getString("PLAYER_LEVEL"), 
		player_->level(),
		player_->str_race());
	Curses::wattroff(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));

	// Basic stats for the player: Health, Attack and Armor points
	Curses::wattron(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Green_Black)));
	Curses::mvwprintw(windows_[2], 5, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("PLAYER_HEALTH"), 
		player_->health_points());
	Curses::mvwprintw(windows_[2], 6, 0, " %s %d | %s %d  ", 
		ResourceManager::getInstance().getString("PLAYER_ATTACK"), 
		player_->attack_points(),
		ResourceManager::getInstance().getString("PLAYER_ARMOR"),
		player_->armor_points());
	Curses::wattroff(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Green_Black)));

	// Experience gained for the player
	Curses::wattron(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));
	Curses::mvwprintw(windows_[2], 8, 0, " %s %d/%d  ", 
		ResourceManager::getInstance().getString("PLAYER_EXPERIENCE"), 
		player_->experience_points(),
		player_->max_experience_points());
	Curses::wattroff(windows_[2], COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));
}

void Game::loadStrings()
{
	// Log window
	ResourceManager::getInstance().addString("DOOR_OPENED",
		"You open the door.");
	ResourceManager::getInstance().addString("CHEST_GATHERED",
		"gold gathered from chest!");
	ResourceManager::getInstance().addString("ATTACK_ENEMY1",
		"You attack enemy:");
	ResourceManager::getInstance().addString("ATTACK_ENEMY2",
		"Damage:");
	ResourceManager::getInstance().addString("ENEMY_LEVEL",
		"Lvl.");

	// Status window
	ResourceManager::getInstance().addString("CHESTS_NUMBER",
		"CHESTS:");
	ResourceManager::getInstance().addString("ENEMIES_NUMBER",
		"ENEMIES:");
	ResourceManager::getInstance().addString("PLAYER_HEALTH",
		"HEALTH:");
	ResourceManager::getInstance().addString("PLAYER_ATTACK",
		"ATTACK:");
	ResourceManager::getInstance().addString("PLAYER_ARMOR",
		"ARMOR:");
	ResourceManager::getInstance().addString("PLAYER_EXPERIENCE",
		"EXPERIENCE:");
	ResourceManager::getInstance().addString("PLAYER_LEVEL",
		"LEVEL:");

	// Map window
	ResourceManager::getInstance().addString("SCORE",
		"SCORE:");
	ResourceManager::getInstance().addString("HI_SCORE",
		"HI-SCORE:");
	ResourceManager::getInstance().addString("EXPLORED",
		"DUNGEON EXPLORED:");
}

int Game::score() const
{
	return score_;
}

int Game::hi_score() const
{
	return hi_score_;
}

void Game::set_score(int score)
{
	score_ = score;
}

void Game::set_hi_score(int hi_score)
{
	hi_score_ = hi_score_;
}

void Game::add_score(int score)
{
	score_ += static_cast<int>(score_factor_ * score);
	if(score_ > hi_score_)
	{
		hi_score_ = score_;
	}
}

double Game::score_factor() const
{
	return score_factor_;
}

bool Game::view_map() const
{
	return view_map_;
}

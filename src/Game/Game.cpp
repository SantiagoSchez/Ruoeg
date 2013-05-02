#include "Game.h"

#include "windows.h"
#include "../../include/ufmod.h"

#include "ResourceManager.h"
#include "../GameObjects/Player/Human/Human.h"
#include "../GameObjects/Player/Orc/Orc.h"
#include "../GameObjects/Player/Elf/Elf.h"
#include "../GameObjects/Player/Dwarf/Dwarf.h"
#include "../GameObjects/Chests/Chest.h"
#include "../Map/Dungeon.h"
#include "../GameObjects/Player/Player.h"

#include <algorithm>
#include <fstream>

Game::Game() : dungeon_(30, 90), score_factor_(0.10), exp_factor_(0.50),
	view_map_(false), deepest_floor_(1), hi_score_(0)
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
	// Setup Curses stuff and dungeon
	setUp();

	// Title screen
	titleScreen();

	// Init the player
	first_location_ = dungeon_.getRandomLit();
	player_->placeIt(first_location_.x, first_location_.y);

	// Game loop
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
	Curses::init_pair(static_cast<int>(GameObject::Color::Magenta_Black), COLOR_MAGENTA, COLOR_BLACK);

	// Load resources
	loadStrings();

	// Init the dungeon
	dungeon_.generate();

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

		if(manageInput(mapWindow()) != -1)
		{
			updateEnemies();
		}

		if(!player_->isAlive())
		{
			end();
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
			Curses::wprintw(consoleWindow(), " GHOST MODE: %d\n", player_->ghost_mode);
		}
		else if(key == static_cast<int>(Curses::Key::F2))
		{
			view_map_ = !view_map_;
			Curses::werase(mapWindow());
			Curses::wprintw(consoleWindow(), " VIEW MAP: %d\n", view_map_);
		}
		else if(key == static_cast<int>(Curses::Key::F3))
		{
			dungeon_.generate();
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
	dungeon_.draw(mapWindow());
	player_->draw(mapWindow());

	// Draw enemies
	for(auto enemy : dungeon_.enemies())
	{
		enemy->draw(mapWindow());
	}

	// Show score and map exploration
	Curses::wattron(mapWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
	Curses::mvwprintw(mapWindow(), 1, 1, " %s %d | %s %d", 
		ResourceManager::getInstance().getString("SCORE"), 
		score_,
		ResourceManager::getInstance().getString("HI_SCORE"),
		hi_score_);
	Curses::mvwprintw(mapWindow(), 1, dungeon_.map().width()-54, "%s -%d",
		ResourceManager::getInstance().getString("CURRENT_FLOOR"),
		dungeon_.floor());
 	Curses::mvwprintw(mapWindow(), 1, dungeon_.map().width()-27, "%s %.2f%%",
 		ResourceManager::getInstance().getString("EXPLORED"),
 		(player_->explored()/(float)dungeon_.map().valid_objects())*100);
	Curses::wattroff(mapWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
}

void Game::updateStatusWindow()
{
	// Number of enemies and chests
	Curses::wattron(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));
	Curses::mvwprintw(statusWindow(), 1, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("ENEMIES_NUMBER"), 
		Enemy::num_enemies());
	Curses::mvwprintw(statusWindow(), 2, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("CHESTS_NUMBER"), 
		Chest::num_chests());
	Curses::wattroff(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));

	// Player level
	Curses::wattron(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
	Curses::mvwprintw(statusWindow(), 4, 0, " %s %d %s  ", 
		ResourceManager::getInstance().getString("PLAYER_LEVEL"), 
		player_->level(),
		player_->str_race());
	Curses::wattroff(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));

	// Basic stats for the player: Health, Attack and Armor points
	Curses::wattron(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Green_Black)));
	Curses::mvwprintw(statusWindow(), 5, 0, " %s %d  ", 
		ResourceManager::getInstance().getString("PLAYER_HEALTH"), 
		player_->health_points());
	Curses::mvwprintw(statusWindow(), 6, 0, " %s %d | %s %d  ", 
		ResourceManager::getInstance().getString("PLAYER_ATTACK"), 
		player_->attack_points(),
		ResourceManager::getInstance().getString("PLAYER_ARMOR"),
		player_->armor_points());
	Curses::wattroff(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Green_Black)));

	// Experience gained for the player
	Curses::wattron(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));
	Curses::mvwprintw(statusWindow(), 8, 0, " %s %d/%d  ", 
		ResourceManager::getInstance().getString("PLAYER_EXPERIENCE"), 
		player_->experience_points(),
		player_->max_experience_points());
	Curses::wattroff(statusWindow(), COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));
}

void Game::loadStrings()
{
	// Log window
	ResourceManager::getInstance().
		addString("DOOR_OPENED", "You open the door.");
	ResourceManager::getInstance().
		addString("CHEST_GATHERED", "gold gathered from chest!");
	ResourceManager::getInstance().
		addString("MAP_OBTAINED", "Map of the dungeon obtained!");
	ResourceManager::getInstance().
		addString("YOU_RECEIVE", "You receive");
	ResourceManager::getInstance().
		addString("YOU_KILLED", "You killed the");
	ResourceManager::getInstance().
		addString("DAMAGE_RECEIVED", "damage!");
	ResourceManager::getInstance().
		addString("EXP_RECEIVED", "experience points!");
	ResourceManager::getInstance().
		addString("GOLD_RECEIVED", "gold!");
	ResourceManager::getInstance().
		addString("ATTACK_ENEMY1", "You attack enemy:");
	ResourceManager::getInstance().
		addString("ATTACK_ENEMY2", "Damage:");
	ResourceManager::getInstance().
		addString("ATTACK_PLAYER1", "Enemy");
	ResourceManager::getInstance().
		addString("ATTACK_PLAYER2", "attacks you:");
	ResourceManager::getInstance().
		addString("ENEMY_LEVEL", "Lvl.");
	ResourceManager::getInstance().
		addString("LVL_UP", "Level up! You reach level");

	// Status window
	ResourceManager::getInstance().
		addString("CHESTS_NUMBER", "CHESTS:");
	ResourceManager::getInstance().
		addString("ENEMIES_NUMBER", "ENEMIES:");
	ResourceManager::getInstance().
		addString("PLAYER_HEALTH", "HEALTH:");
	ResourceManager::getInstance().
		addString("PLAYER_ATTACK", "ATTACK:");
	ResourceManager::getInstance().
		addString("PLAYER_ARMOR", "ARMOR:");
	ResourceManager::getInstance().
		addString("PLAYER_EXPERIENCE", "EXPERIENCE:");
	ResourceManager::getInstance().
		addString("PLAYER_LEVEL", "LEVEL:");

	// Map window
	ResourceManager::getInstance().
		addString("SCORE", "SCORE:");
	ResourceManager::getInstance().
		addString("HI_SCORE", "HI-SCORE:");
	ResourceManager::getInstance().
		addString("EXPLORED", "DUNGEON EXPLORED:");
	ResourceManager::getInstance().
		addString("CURRENT_FLOOR", "CURRENT FLOOR:");
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

int Game::deepest_floor() const
{
	return deepest_floor_;
}

void Game::set_deepest_floor(int deepest_floor)
{
	deepest_floor_ = deepest_floor;
}

void Game::add_score(int score)
{
	int score_to_add = static_cast<int>(score_factor_ * score);

	if(score_to_add <= 0)
	{
		score_to_add = 1;
	}
	
	score_ += score_to_add;
	
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

void Game::set_view_map(bool view_map)
{
	view_map_ = view_map;
}

double Game::exp_factor() const
{
	return exp_factor_;
}

void Game::increase_factors()
{
	score_factor_ += 0.15;
	exp_factor_ += 0.50;
}

void Game::end()
{
	WINDOW *prompt = Curses::derwin(mapWindow(), 
		12, 45, dungeon_.map().height()/2-5, dungeon_.map().width()/2-22);
	Curses::wbkgd(prompt, COLOR_PAIR(GameObject::Color::Red_Black));

	saveData();

	while(prompt != NULL)
	{
		Curses::mvwprintw(prompt, 1, 1, "Congratulations %s, you are dead!           ", player_->str_race());
		Curses::mvwprintw(prompt, 2, 1, "                                            ");
		Curses::mvwprintw(prompt, 3, 1, "But don't worry, you have reached           ");
		Curses::mvwprintw(prompt, 4, 1, "the floor -%d in the dungeon!               ", dungeon_.floor());
		Curses::mvwprintw(prompt, 5, 1, "                                            ");
		Curses::mvwprintw(prompt, 6, 1, "Your HI-SCORE is saved, so don't be scared  ");
		Curses::mvwprintw(prompt, 7, 1, "of going through the dungeon again brave    ");
		Curses::mvwprintw(prompt, 8, 1, "warrior!                                    ");
		Curses::mvwprintw(prompt, 9, 1, "                                            ");
		Curses::mvwprintw(prompt, 10, 1, "       <PRESS [ENTER] TO CONTINUE>          ");

		Curses::wbox(prompt, 0, 0);
		Curses::wrefresh(prompt);

		int key = Curses::wgetch(prompt);
		if(key != -1)
		{
			if(key == static_cast<int>(Curses::Key::Space))
			{
				Curses::delwin(prompt);

				return;
			}
		}
	}
}

void Game::titleScreen()
{
	WINDOW *win = Curses::newwin(40, 90, 0, 0);

	loadData();

	while(win != NULL)
	{
		Curses::wattron(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));
		Curses::mvwprintw(win,  1, 0, "              ,-.----.                                                                   ");
		Curses::mvwprintw(win,  2, 0, "              \\    /  \\                                                                  ");
		Curses::mvwprintw(win,  3, 0, "              ;   :    \\           ,--,     ,---.                                        ");
		Curses::mvwprintw(win,  4, 0, "              |   | .\\ :         ,'_ /|    '   ,'\\               ,----._,.               ");
		Curses::mvwprintw(win,  5, 0, "              .   : |: |    .--. |  | :   /   /   |    ,---.    /   /  ' /               ");
		Curses::mvwprintw(win,  6, 0, "              |   |  \\ :  ,'_ /| :  . |  .   ; ,. :   /     \\  |   :     |               ");
		Curses::mvwprintw(win,  7, 0, "              |   : .  /  |  ' | |  . .  '   | |: :  /    /  | |   | .\\  .               ");
		Curses::mvwprintw(win,  8, 0, "              ;   | |  \\  |  | ' |  | |  '   | .; : .    ' / | .   ; ';  |               ");
		Curses::mvwprintw(win,  9, 0, "              |   | ;\\  \\ :  | : ;  ; |  |   :    | '   ;   /| '   .   . |               ");
		Curses::mvwprintw(win, 10, 0, "              :   ' | \\.' '  :  `--'   \\  \\   \\  /  '   |  / |  `---`-'| |               ");
		Curses::mvwprintw(win, 11, 0, "              :   : :-'   :  ,      .-./   `----'   |   :    |  .'__/\\_: |               ");
		Curses::mvwprintw(win, 12, 0, "              |   |.'      `--`----'                 \\   \\  /   |   :    :               ");
		Curses::mvwprintw(win, 13, 0, "              `---'                                   `----'     \\   \\  /                ");
		Curses::mvwprintw(win, 14, 0, "                                                                  `--`-'                 ");
		Curses::mvwprintw(win, 16, 30, "~ A minimalist roguelike game ~");
		Curses::wattroff(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Red_Black)));

		Curses::wattron(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));
		Curses::mvwprintw(win, 20, 30, "Choose your race, brave warrior!");
		Curses::wattroff(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Cyan_Black)));

		Curses::mvwaddch(win, 22, 29, '@');
		Curses::mvwprintw(win, 23, 26, "[H]uman");
		Curses::mvwaddch(win, 22, 41, '@' | COLOR_PAIR(static_cast<int>(GameObject::Color::White_Yellow)));
		Curses::mvwprintw(win, 23, 39, "[E]lf");
		Curses::mvwaddch(win, 22, 51, '@' | COLOR_PAIR(static_cast<int>(GameObject::Color::White_Green)));
		Curses::mvwprintw(win, 23, 49, "[O]rc");
		Curses::mvwaddch(win, 22, 62, '@' | COLOR_PAIR(static_cast<int>(GameObject::Color::White_Red)));
		Curses::mvwprintw(win, 23, 59, "[D]warf");

		Curses::wattron(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));
		Curses::mvwprintw(win, 30, 30, "Deepest floor: -%d | Best score: %d", deepest_floor_, hi_score_);
		Curses::wattroff(win, COLOR_PAIR(static_cast<int>(GameObject::Color::Yellow_Black)));

		Curses::mvwprintw(win, 39, 0, "Copyright (c) 2013 Santiago Sanchez Sobrino.");
		Curses::wrefresh(win);

		int key = Curses::wgetch(win);
		if(key != -1)
		{
			if((key == 'h') || (key == 'H'))
			{
				Curses::delwin(win);
				player_.reset(new Human(dungeon_));
				Curses::flash();
				return;
			}
			else if((key == 'e') || (key == 'E'))
			{
				Curses::delwin(win);
				player_.reset(new Elf(dungeon_));
				Curses::flash();
				return;
			}
			else if((key == 'o') || (key == 'O'))
			{
				Curses::delwin(win);
				player_.reset(new Orc(dungeon_));
				Curses::flash();
				return;
			}
			else if((key == 'd') || (key == 'D'))
			{
				Curses::delwin(win);
				player_.reset(new Dwarf(dungeon_));
				Curses::flash();
				return;
			}
		}
	}
}

bool Game::saveData()
{
	std::ofstream file("data.ruoeg", std::ios::out | std::ios::out | std::ios::binary);

	if(!file)
	{
		return false;
	}

	file.seekp(0);
	file.write(reinterpret_cast<const char*>(&hi_score_), sizeof(hi_score_));
	file.write(reinterpret_cast<const char*>(&deepest_floor_), sizeof(deepest_floor_));

	return true;
}

bool Game::loadData()
{
	std::ifstream file("data.ruoeg", std::ios::in | std::ios::binary);
	
	file.seekg(0, file.end);
	if((!file) || (file.tellg() <= 0))
	{
		return false;
	}
	file.seekg(0, file.beg);

	file.read(reinterpret_cast<char*>(&hi_score_), sizeof(hi_score_));
	file.read(reinterpret_cast<char*>(&deepest_floor_), sizeof(deepest_floor_));

	return true;
}

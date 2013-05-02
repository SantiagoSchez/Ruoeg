//
// Game class following a singleton pattern, as we only need
// one instance of the class.
// 

#ifndef RUOEG_GAME_GAME_H_
#define RUOEG_GAME_GAME_H_

#include <vector>
#include <memory>

#include "../Curses/Curses.h"
#include "../Map/Dungeon.h"
#include "../GameObjects/Player/Player.h"

class Game
{
public:
	static Game& getInstance();
	~Game();

	void start();
	void setUp();
	void loop(Curses::Key exit_key = Curses::Key::Esc);
	void refreshWindows(std::vector<WINDOW *> windows);
	int manageInput(WINDOW *win);
	void loadStrings();
	void titleScreen();

	void updateEnemies();
	void updateEnemyDeaths();
	void updateMapWindow();
	void updateStatusWindow();

	WINDOW* mapWindow();
	WINDOW* consoleWindow();
	WINDOW* statusWindow();

	int score() const;
	int hi_score() const;
	void set_score(int score);
	void set_hi_score(int hi_score);
	void add_score(int score);
	int deepest_floor() const;
	void set_deepest_floor(int deepest_floor);

	double score_factor() const;
	double exp_factor() const;
	void increase_factors();

	bool view_map() const;
	void set_view_map(bool view_map);

	void end();

	bool saveData();
	bool loadData();

private:
	enum class State 
	{
		Finish, 
		Running
	};

	Game();

	State state_;
	Dungeon dungeon_;
	std::unique_ptr<Player> player_;
	std::vector<WINDOW *> windows_;
	int score_, hi_score_, deepest_floor_;
	double score_factor_, exp_factor_;
	bool view_map_;
	Point first_location_;

	// Disallowing copy constructor and assignment operator
	Game(Game const&);
	Game& operator=(Game const&);
};

#endif // RUOEG_GAME_GAME_H_
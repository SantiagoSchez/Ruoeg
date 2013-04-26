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

	void start();
	void setUp();
	void loop(Curses::Key exit_key = Curses::Key::Esc);
	void refreshWindows(std::vector<WINDOW *> windows);
	void release();
	int manageInput(WINDOW *win);
	void loadStrings();

	void updateMapWindow();
	void updateStatusWindow();

	WINDOW* mapWindow();
	WINDOW* consoleWindow();
	WINDOW* statusWindow();

	int current_score_value() const;
	int score() const;
	int hi_score() const;
	void set_score(int score);
	void set_hi_score(int hi_score);
	void add_score(double factor);

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
	int score_, hi_score_, current_score_value_;

	// Disallowing copy constructor and assignment operator
	Game(Game const&);
	Game& operator=(Game const&);
};

#endif // RUOEG_GAME_GAME_H_
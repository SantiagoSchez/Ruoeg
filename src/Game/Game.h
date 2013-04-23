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

	WINDOW* mapWindow();
	WINDOW* consoleWindow();
	WINDOW* statusWindow();

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

	// Disallowing copy constructor and assignment operator
	Game(Game const&);
	Game& operator=(Game const&);
};

#endif // RUOEG_GAME_GAME_H_
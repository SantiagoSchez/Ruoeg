//
// Game class following a singleton pattern, as we only need
// one instance of the class.
// 

#ifndef RUOEG_GAME_GAME_H_
#define RUOEG_GAME_GAME_H_

#include <vector>

#include "../Crs/Crs.h"
#include "../Map/Dungeon.h"

class Game
{
public:
	static Game& getInstance();

	void start();
	void setUp();
	void loop(Crs::Key exit_key = Crs::Key::Esc);
	void refreshWindows(std::vector<WINDOW *> windows);
	void release();
	void manageInput(WINDOW *win);

private:
	enum class State 
	{
		Finish, 
		Running
	};

	Game();

	State state_;
	Dungeon dungeon_;
	std::vector<WINDOW *> windows_;

	// Disallowing copy constructor and assignment operator
	Game(Game const&);
	Game& operator=(Game const&);
};

#endif // RUOEG_GAME_GAME_H_
//
// Set the curses library inside the namespace Curses
// 

#ifndef RUOEG_CURSES_CURSES_H_
#define RUOEG_CURSES_CURSES_H_

#include "../../include/curses.h"

class Curses
{
public:
	static enum class Key 
	{
		Down = 0x102, 
		Up = 0x103, 
		Left = 0x104, 
		Right = 0x105, 
		Esc = 0x1B,
		Space = 0x20,
		F1 = KEY_F(1),
		F2 = KEY_F(2),
		F3 = KEY_F(3),
		F4 = KEY_F(4)
	};

	// Starts curses mode
	static WINDOW* initscr();

	// Resize a given curses window
	static int wresize(WINDOW *win, int lines, int columns);

	// Resize the terminal
	static int resize_term(int lines, int columns);

	// Whether cursor displays (1) or not (0)
	static int curs_set(int visibility);

	// Allows the input of function keys (F1, F2, Up arrow, ...)
	static int keypad(WINDOW *win, bool bf);

	// No echoing
	static int noecho();

	// No buffering
	static int cbreak();

	// No delaying
	static int nodelay(WINDOW *win, bool bf);

	// Enables/Disables scroll
	static int scrollok(WINDOW *win, bool bf);

	// Ends curses mode
	static void endwin();

	// Output 'ch' at (x,y) in win
	static int mvwaddch(WINDOW *win, int y, int x, const chtype ch);

	// Output a formatted str in win
	static int wprintw(WINDOW *win, char *fmt, ...);

	// Output a formatted str in win in (x, y)
	static int mvwprintw(WINDOW *win, int y, int x, char *fmt, ...);

	// Wait for key input
	static int wgetch(WINDOW *win);

	// Starts color mode
	static int start_color();

	// Enables attributes to a window
	static int wattron(WINDOW *win, int attrs);

	// Disable attributes to a window
	static int wattroff(WINDOW *win, int attrs);

	// Creates a new pair (1..COLOR_PAIRS-1) with the given f(oreground) 
	// and b(ackground) color
	static int init_pair(short pair, short f, short b);

	// Returns true if terminal can display colors
	static int has_colors();

	// Creates a new window
	static WINDOW* newwin(int nlines, int ncols, int begin_y, int begin_x);

	// Deletes a given window
	static int delwin(WINDOW *win);

	// Surround a window with a frame
	static int wbox(WINDOW *win, chtype verch, chtype horch);

	// Refresh the given window
	static int wrefresh(WINDOW *win);

	// Refresh but doesn't update the screen
	static int wnoutrefresh(WINDOW *win);

	// Updates the screen
	static int doupdate();

	// Emits a system sound
	static int beep();

	// Flash the window
	static int flash();

	// Wait ms milliseconds
	static int napms(int ms);

	// Clear the given window
	static int werase(WINDOW *win);
};

#endif // RUOEG_CURSES_CURSES_H_
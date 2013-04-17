#include "Crs.h"

WINDOW* Crs::initscr()
{
	return ::initscr();
}

int Crs::wresize(WINDOW *win, int lines, int columns)
{
	return ::wresize(win, lines, columns);
}

int Crs::resize_term(int lines, int columns)
{
	return ::resize_term(lines, columns);
}

int Crs::curs_set(int visibility)
{
	return ::curs_set(visibility);
}

int Crs::keypad(WINDOW *win, bool bf)
{
	return ::keypad(win, bf);
}

int Crs::noecho()
{
	return ::noecho();
}

int Crs::cbreak()
{
	return ::cbreak();
}

int Crs::nodelay(WINDOW *win, bool bf)
{
	return ::nodelay(win, bf);
}

int Crs::scrollok(WINDOW *win, bool bf)
{
	return ::scrollok(win, bf);
}

void Crs::endwin()
{
	::endwin();
}

int Crs::mvwaddch(WINDOW *win, int y, int x, const chtype ch)
{
	return ::mvwaddch(win, y, x, ch);
}

int Crs::mvwaddstr(WINDOW *win, int y, int x, const char *str)
{
	return ::mvwaddstr(win, y, x, str);
}

int Crs::waddstr(WINDOW *win, const char *str)
{
	return ::waddstr(win, str);
}

int Crs::wgetch(WINDOW *win)
{
	return ::wgetch(win);
}

int Crs::start_color()
{
	return ::start_color();
}

int Crs::wattron(WINDOW *win, int attrs)
{
	return ::wattron(win, attrs);
}

int Crs::wattroff(WINDOW *win, int attrs)
{
	return ::wattroff(win, attrs);
}

int Crs::init_pair(short pair, short f, short b)
{
	return ::init_pair(pair, f, b);
}

int Crs::has_colors()
{
	return ::has_colors();
}

WINDOW* Crs::newwin(int nlines, int ncols, int begin_y, int begin_x)
{
	return ::newwin(nlines, ncols, begin_y, begin_x);
}

int Crs::delwin(WINDOW *win)
{
	return ::delwin(win);
}

int Crs::box(WINDOW *win, chtype verch, chtype horch)
{
	return ::box(win, verch, horch);
}

int Crs::refresh(WINDOW *win)
{
	return ::wrefresh(win);
}
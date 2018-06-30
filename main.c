#include "snake.h"

// Set up/clean up curses stuff
void initCurses();
void cleanupCurses();

int main(int argc, char** argv)
{
	initCurses();

	SnakeSettings settings = DEFAULT_SNAKE_SETTINGS;
	startSnake(&settings);

	cleanupCurses();
}

void initCurses()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	refresh();
}

void cleanupCurses()
{
	endwin();
}

#include "snake.h"
#include "assert.h"

// Set up/clean up curses stuff
void initCurses();
void cleanupCurses();

int main()
{
	initCurses();

	SnakeSettings settings = DEFAULT_SNAKE_SETTINGS;
	assert(settings.speed_increase == 0);
	settings.initial_snake_delay = 70,
	settings.minimum_snake_delay = 20,
	settings.snake_delay_ratio = 1.4;
	settings.speed_increase = 1;
	settings.speed_increase_period = 30000;
	
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

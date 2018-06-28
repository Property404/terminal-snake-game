#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include "timer.h"
#include "point.h"
#include "vector.h"
const int MEMORY_EXPANSE_RATE = 20;// Changing this variable effects optimization, but not behavior
const char SNAKE_HEAD = 'O';
const char SNAKE_FOOD = '$';
const int MAX_FOODS = 4;

// Set up/clean up curses stuff
void initGame();
void cleanupGame();

// Utility function that we can't really decouple
// Find an equivalent point in a vector
// Return -1 if unable to find, else return the index
int findPointInVector(Vector* v, Point* p)
{
	for(int i=0;i<v->length;i++)
	{
		Point* vp = accessVector(v, i);
		if(vp->x == p->x && vp->y == p->y)
			return i;
	}
	return -1;
}

// Move (and draw) snake in specific direction
void advanceSnake(Vector* snake, Direction direction);


int main()
{
	initGame();

	Direction direction = DOWN;

	Timer snake_vertical_timer = constructTimer(75);
	Timer snake_horizontal_timer = constructTimer(40);
	Timer food_timer = constructTimer(1000);

	Vector snake_parts =
		constructComplexVector(MEMORY_EXPANSE_RATE);
	Vector foods =
		constructComplexVector(MEMORY_EXPANSE_RATE);

	// Make head
	pushOntoVector(&snake_parts, constructPointDynamically(0,0));

	while(true)
	{
		timeout(0);
		switch(getch())
		{
			case KEY_UP:
				direction = UP;
				break;
			case KEY_RIGHT:
				direction = RIGHT;
				break;
			case KEY_LEFT:
				direction = LEFT;
				break;
			case KEY_DOWN:
				direction = DOWN;
				break;
			default:
				break;
		}

		// Here we create random food particles for the snake to eat
		if(checkTimer(&food_timer) && foods.length < MAX_FOODS)
		{
			int max_y, max_x;
			getmaxyx(stdscr, max_y, max_x);

			Point* new_food = constructPointDynamically(0, 0);
			do
			{
				new_food->x = rand() % max_x;
				new_food->y = rand() % max_y;
			} while(findPointInVector(&snake_parts, new_food) != -1);

			mvaddch(new_food->y, new_food->x, SNAKE_FOOD);

			pushOntoVector(&foods, new_food);
		}

		// Here we are handling the movement of the snake
		// Speed is affected by direction, due to the fact that
		// vertical movement will generally appear to be faster
		if(direction == UP || direction == DOWN ?
				checkTimer(&snake_vertical_timer):
				checkTimer(&snake_horizontal_timer)
				)
		{

			advanceSnake(&snake_parts, direction);
			int food_index  = findPointInVector(
					&foods,
					accessVector(&snake_parts, 0)
					);
			if(food_index != -1)
			{
				pushOntoVector(&snake_parts,
						constructPointDynamically(0,0));
			}
		}

	}

	destroyVector(&snake_parts);
	destroyVector(&foods);
	cleanupGame();

}

void advanceSnake(Vector* snake, Direction direction)
{
	assert(snake->length > 0);
	assert(accessVector(snake, 0) != NULL);

	Point new_pos = *((Point*)(accessVector(snake, 0)));
	advancePoint(&new_pos, direction);

	pushInFrontOfVector(snake,
			constructPointDynamically(
				new_pos.x, new_pos.y
			)
		);

	// Cover up and delete last body part
	Point* old_pos = accessVector(snake, snake->length - 1);
	mvaddch(old_pos->y, old_pos->x, ' ');
	popOffVector(snake);


	// Aaaaaaand draw
	mvaddch(new_pos.y, new_pos.x, SNAKE_HEAD);
	for(int i=1;i<snake->length;i++)
	{
		Point* p = accessVector(snake, i);
		Point* prev_p = accessVector(snake, i-1);
		mvaddch(p->y, p->x,
				// Display a different character based on going vertically or horizontally
				(p->x == prev_p->x)?  '|':'-');
						
	}
	refresh();
}

void initGame()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	refresh();
}

void cleanupGame()
{
	endwin();
}

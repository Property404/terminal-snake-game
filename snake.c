#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include "timer.h"
#include "point.h"
#include "vector.h"
#include "snake.h"
typedef SnakeSettings Settings;

// On screen characters
const char SNAKE_HEAD = 'O';
const char SNAKE_BODY_VERTICAL = '|';
const char SNAKE_BODY_HORIZONTAL = '-';
const char SNAKE_FOOD = '$';

// Utility function that we can't really decouple
// Find an equivalent point in a vector
// Return -1 if unable to find, else return the index
int findPointInVector(Vector* v, Point* p);

// Move in the direction of "key"(being an arrow key)
// But don't allow moving backwards directly
void mapKeyToDirection(int key, Direction* direction);

// Move (and draw) snake in specific direction
void advanceSnake(Vector* snake, Direction direction);

// Add new food item to game
void addFood(Vector* foods, Vector* snake);

long getVerticalDelay(Settings* settings, long delay);
long getHorizontalDelay(Settings* settings, long delay);


void startSnake(SnakeSettings* settings)
{
	if(settings->window==NULL)
		settings->window=stdscr;

	Direction direction = DOWN;
	long snake_delay = settings->initial_snake_delay;

	Timer snake_vertical_timer = constructTimer(
			getVerticalDelay(settings, snake_delay));
	Timer snake_horizontal_timer = constructTimer(
			getHorizontalDelay(settings, snake_delay));
	Timer food_timer = constructTimer(settings->food_delay);

	// Make initial snake
	Vector snake_parts =
		constructComplexVector(settings->memory_expanse_rate);
	pushOntoVector(&snake_parts, constructPointDynamically(0,0));

	// Initial food
	Vector foods =
		constructComplexVector(settings->memory_expanse_rate);
	addFood(&foods, &snake_parts);


	while(true)
	{
		timeout(0);
		int c = getch();
		switch(c)
		{
			case KEY_UP:
			case KEY_RIGHT:
			case KEY_LEFT:
			case KEY_DOWN:
				mapKeyToDirection(c, &direction);
				break;
			default:
				break;
		}

		// Here we create random food particles for the snake to eat
		if(checkTimer(&food_timer) && foods.length < settings->max_foods)
		{
			addFood(&foods, &snake_parts);
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
				removeFromVector(&foods, food_index);
			}
		}

	}

	destroyVector(&snake_parts);
	destroyVector(&foods);
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


	// Draw new head
	mvaddch(new_pos.y, new_pos.x, SNAKE_HEAD);

	// And new neck
	if(snake->length > 1)
	{
		Point* p = accessVector(snake, 1);
		Point* prev_p = accessVector(snake, 0);
		// Display a different character based on going vertically or
		// horizontally
		mvaddch(p->y, p->x, (p->x == prev_p->x)?  '|':'-');
						
	}
	refresh();
}

void mapKeyToDirection(int key, Direction* direction)
{
	const Direction directions[] =
	{DOWN, UP, LEFT, RIGHT};
	const int index = key - KEY_DOWN;
	if (index > 3 || index < 0)
	{
		fprintf(stderr, "Out of bounds in mapKeyToDirection()\n");
		exit(1);
	}
	Direction new_direction = directions[index];
	if(*direction == -new_direction)
		return;
	*direction = new_direction;
}

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

void addFood(Vector* foods, Vector* snake)
{
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

	Point* new_food = constructPointDynamically(0, 0);
	do
	{
		new_food->x = rand() % max_x;
		new_food->y = rand() % max_y;
	} while(findPointInVector(snake, new_food) != -1);

	mvaddch(new_food->y, new_food->x, SNAKE_FOOD);

	pushOntoVector(foods, new_food);
}

long getVerticalDelay(Settings* settings, long delay)
{
	return delay;
}
long getHorizontalDelay(Settings* settings, long delay)
{
	return delay * settings->snake_delay_ratio;
}

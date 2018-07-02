#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include "highscore.h"
#include "timer.h"
#include "point.h"
#include "vector.h"
#include "snake.h"
#include "error.h"
typedef SnakeSettings Settings;

// On screen characters
static const char SNAKE_HEAD = 'O';
static const char SNAKE_BODY_VERTICAL = '|';
static const char SNAKE_BODY_HORIZONTAL = '-';
static const char SNAKE_FOOD = '$';

// Score parameters
static const int MIN_SCORE_INCREASE = 5;
static const int MAX_SCORE_INCREASE = 70;

// Utility function that we can't really decouple
// Find an equivalent point in a vector
// Return -1 if unable to find, else return the index
static int findPointInVector(const Vector* v, const Point* p);
// Same but in reverse order
static int rfindPointInVector(const Vector* v, const Point* p);

// Move in the direction of "key"(being an arrow key)
// But don't allow moving backwards directly
static void mapKeyToDirection(int key, Direction* direction);

// Move (and draw) snake in specific direction
static void advanceSnake(const Settings*, Vector* snake, Direction direction);

// Add new food item to game
static void addFood(const Settings*, Vector* foods, const Vector* snake);

static long getVerticalDelay(const Settings*, long delay);
static long getHorizontalDelay(const Settings* , long delay);

static bool hasLost(const Settings*, const Vector* snake);

static int getMaxWidth(const Settings* s);
static int getMaxHeight(const Settings* s);
static void showScore(const Settings* s, int score);



int startSnake(SnakeSettings* settings)
{
	if(settings->window==NULL)
		settings->window=stdscr;

	srand(time(NULL));

	timeout(0);

	Direction direction = DOWN;
	long snake_delay = settings->initial_snake_delay;

	Timer snake_vertical_timer = constructTimer(
			getVerticalDelay(settings, snake_delay));
	Timer snake_horizontal_timer = constructTimer(
			getHorizontalDelay(settings, snake_delay));
	Timer food_timer = constructTimer(settings->food_delay);
	Timer speed_increase_timer =
		constructTimer(settings->speed_increase_period);

	int score = 0;
	showScore(settings, score);

	// Make initial snake
	Vector snake_parts =
		constructComplexVector(settings->memory_expanse_rate);
	pushOntoVector(&snake_parts, constructPointDynamically(0,0));

	// Initial food
	Vector foods =
		constructComplexVector(settings->memory_expanse_rate);
	addFood(settings, &foods, &snake_parts);


	while(true)
	{
		int c = getch();
		switch(c)
		{
			case KEY_UP:
			case KEY_RIGHT:
			case KEY_LEFT:
			case KEY_DOWN:
				mapKeyToDirection(c, &direction);
				break;
			case 'p':
			case 'P':
			case ' ':
				timeout(-1);
				c=getch();
				timeout(0);
			default:
				break;
		}

		// Let's go faster
		if(checkTimer(&speed_increase_timer) && snake_delay >
				settings->minimum_snake_delay)
		{
			snake_delay -= settings->speed_increase;

			setTimerDelay(&snake_vertical_timer,
				getVerticalDelay(settings, snake_delay));
			setTimerDelay(&snake_horizontal_timer, 
				getHorizontalDelay(settings, snake_delay));
		}

		// Here we create random food particles for the snake to eat
		if(checkTimer(&food_timer) && foods.length < settings->max_foods)
		{
			addFood(settings, &foods, &snake_parts);

			int score_increase = MAX_SCORE_INCREASE-snake_delay;
			if(score_increase<MIN_SCORE_INCREASE)
				score_increase = MIN_SCORE_INCREASE;
			score+=score_increase;

			showScore(settings, score);
		}

		// Here we are handling the movement of the snake
		// Speed is affected by direction, due to the fact that
		// vertical movement will generally appear to be faster
		if(direction == UP || direction == DOWN ?
				checkTimer(&snake_vertical_timer):
				checkTimer(&snake_horizontal_timer)
				)
		{

			advanceSnake(settings, &snake_parts, direction);
			const Point* head_pos = accessVector(&snake_parts, 0);

			// Lost?
			if(hasLost(settings, &snake_parts))
				break;

			// See if we gobbled any food
			const int food_index  = findPointInVector(
					&foods,
					head_pos
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
	return score;
}

static void advanceSnake(const Settings* s, Vector* snake, Direction direction)
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
	mvwaddch(s->window, old_pos->y, old_pos->x, ' ');
	popOffVector(snake);


	// Draw new head
	mvwaddch(s->window, new_pos.y, new_pos.x, SNAKE_HEAD);

	// And new neck
	if(snake->length > 1)
	{
		Point* p = accessVector(snake, 1);
		Point* prev_p = accessVector(snake, 0);
		// Display a different character based on going vertically or
		// horizontally
		mvwaddch(s->window, p->y, p->x, (p->x == prev_p->x)?
				SNAKE_BODY_VERTICAL : SNAKE_BODY_HORIZONTAL);
						
	}
	refresh();
}

static void mapKeyToDirection(int key, Direction* direction)
{
	const Direction directions[] =
	{DOWN, UP, LEFT, RIGHT};
	const int index = key - KEY_DOWN;

	errorIf(index>3 || index<0,
		"Out of bounds in mapKeyToDirection()");

	Direction new_direction = directions[index];
	if(*direction == -new_direction)
		return;
	*direction = new_direction;
}

static inline bool matchesPointInVector(const Vector* v, const Point* p, int i)
{
	Point* vp = accessVector(v, i);
	if(vp->x == p->x && vp->y == p->y)
		return true;
	return false;
}
static int findPointInVector(const Vector* v, const Point* p)
{
	for(int i=0;i<v->length;i++)
	{
		if(matchesPointInVector(v, p, i))
			return i;
	}
	return -1;
}

static int rfindPointInVector(const Vector*v, const Point* p)
{
	for(int i=v->length -1; i>=0;i--)
	{
		if(matchesPointInVector(v, p, i))
			return i;
	}
	return -1;
}

static void addFood(const Settings* s, Vector* foods, const Vector* snake)
{
	const int max_x = getMaxWidth(s);
	const int max_y = getMaxHeight(s);

	Point* new_food = constructPointDynamically(0, 0);
	do
	{
		new_food->x = rand() % (max_x-2) + 1;
		new_food->y = rand() % (max_y-2) + 1;
	} while(findPointInVector(snake, new_food) != -1);

	mvwaddch(s->window, new_food->y, new_food->x, SNAKE_FOOD);

	pushOntoVector(foods, new_food);
}

static long getVerticalDelay(const Settings* s, long delay)
{
	return delay * s->snake_delay_ratio;
}
static long getHorizontalDelay(const Settings* s, long delay)
{
	(void)s;//silence warning
	return delay;
}

static bool hasLost(const Settings* s, const Vector* snake)
{
	const int max_x = getMaxWidth(s);
	const int max_y = getMaxHeight(s);

	const Point* head_pos = accessVector(snake, 0);
	if(
			rfindPointInVector(snake, head_pos)
			|| (head_pos->x<0 || head_pos->x>=max_x)
			|| (head_pos->y<0 || head_pos->y>=max_y)
	  )
	{
		return true;
	}
	return false;
}

#define SCORE_BAR_SIZE 2

static void displayString(const Settings* s, int x, int y,
		const char* format, ...)
{
	va_list argptr;
	va_start (argptr, format);
	wmove(s->window, y, x);
	vwprintw(s->window,format, argptr);
	va_end(argptr);
}

static void drawScoreBar(const Settings* s)
{
	// Only do this once
	static bool drawn = false;
	if(drawn)
		return;
	drawn = true;

	// Draw seperator line
	const int max_width = getMaxWidth(s);
	char line[max_width+1];
	for(int i=0;i<=max_width;i++)
	{
		line[i] = '=';
	}
	line[max_width] = '\0';
	displayString(s, 0, getMaxHeight(s), line);
	displayString(s, 0, getMaxHeight(s)+1, "High Score: %d",
			getHighScore());
}

static void showScore(const Settings* s, int score)
{
	drawScoreBar(s);
	const int score_size = 12;

	displayString(s, getMaxWidth(s)-score_size, getMaxHeight(s)+1, "Score: %d ", score);
}
	
static int getMaxWidth(const Settings* s)
{
	static int x = -1;
	if(x == -1)
	{
		int y;
		getmaxyx(s->window, y, x);
		(void)y;
	}
	return x;
}
static int getMaxHeight(const Settings* s)
{
	static int y = -1;
	if(y == -1)
	{
		int x;
		getmaxyx(s->window, y, x);
		(void)x;
	}
	return y-SCORE_BAR_SIZE;
}

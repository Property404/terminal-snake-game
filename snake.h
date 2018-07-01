#pragma once
#include "curses.h"

typedef struct
{
	// In which window the game is played
	WINDOW* window;
	// How slow is the snake(initially)
	long initial_snake_delay;
	long minimum_snake_delay;
	long speed_increase;
	long speed_increase_period;
	// Ratio between vertical delay and horizontal delay
	// (Should be above 1)
	float snake_delay_ratio;
	// How long does it take food to appear?
	long food_delay;
	// How many foods can appear on the screen?
	int max_foods;

	// Memory expanse rate for vectors
	int memory_expanse_rate;

} SnakeSettings;

static const SnakeSettings DEFAULT_SNAKE_SETTINGS = 
{
	.window = NULL,
	.initial_snake_delay = 50,
	.minimum_snake_delay = 10,
	.speed_increase = 0,
	.speed_increase_period = 50000,
	.snake_delay_ratio = 1.5,
	.food_delay = 500,
	.max_foods = 1,
	.memory_expanse_rate = 20
};

// Start the game
// Returns score
int startSnake(SnakeSettings*);

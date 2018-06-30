#pragma once
#include "curses.h"

typedef struct
{
	// In which window the game is played
	WINDOW* window;
	// How slow is the snake(initially)
	long initial_snake_delay;
	// Ratio between horizontal delay and vertical delay
	// (Should be below 1)
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
	.initial_snake_delay = 75,
	.snake_delay_ratio = .5,
	.food_delay = 500,
	.max_foods = 1,
	.memory_expanse_rate = 20
};

// Start the game
void startSnake(SnakeSettings*);

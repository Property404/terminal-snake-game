#pragma once

// Represents direction something is headed on screen
typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

// Represents a point on the screen in terms of X, Y
typedef struct {int x; int y;} Point;

Point constructPoint(int x, int y);
Point* constructPointDynamically(int x, int y);

// Increment point in specified direction
void advancePoint(Point* p, Direction d);

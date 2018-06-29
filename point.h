#pragma once

// Represents direction something is headed on screen
// Properties:
// 	* abs values of vertical directions are 1
//  * abs values of horizontal directions are 2
// 	* UP = -DOWN and vice versa
// 	* LEFT = -RIGHT and vice versa
typedef enum {UP = -1, DOWN = 1, LEFT = -2, RIGHT = 2} Direction;

// Represents a point on the screen in terms of X, Y
typedef struct {int x; int y;} Point;

Point constructPoint(int x, int y);
Point* constructPointDynamically(int x, int y);

// Increment point in specified direction
void advancePoint(Point* p, Direction d);

#include "point.h"
#include <stdlib.h>

Point constructPoint(int x, int y)
{
	Point p = {.x = x, .y = y};
	return p;
}
Point* constructPointDynamically(int x, int y)
{
	Point* p = malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	return p;
}
void advancePoint(Point* p, Direction d)
{
	switch(d)
	{
		case UP:
			p->y --;
			break;
		case DOWN:
			p->y ++;
			break;
		case LEFT:
			p->x --;
			break;
		case RIGHT:
			p->x ++;
			break;
	}
}

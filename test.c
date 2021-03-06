#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "vector.h"
#include "point.h"

// Get data from vector with index as int
long getLong(Vector* v, int index)
{
	return (long)v->data[index];
}
long getLastLong(Vector* v)
{
	return getLong(v, v->length - 1);
}

int main()
{
	//Make sure timer works Properly
	{
		Timer t = constructTimer(500);
		Timer t2 = constructTimer(50000);
		assert(checkTimer(&t) == false);
		assert(checkTimer(&t2) == false);
		sleep(1);
		assert(checkTimer(&t));
		assert(checkTimer(&t) == false);
		assert(checkTimer(&t2) == false);
	}

	// Make sure vector works properly
	{
		Vector v = constructSimpleVector(1);

		pushOntoVector(&v, (void*)3);
		assert(getLastLong(&v) == 3);

		pushOntoVector(&v, (void*)1);
		assert(getLastLong(&v) == 1);

		pushOntoVector(&v, (void*)5);
		assert(getLastLong(&v) == 5);

		popOffVector(&v);
		assert(getLastLong(&v) == 1);

		destroyVector(&v);
	}

	//Make sure points work vector
	{
		Vector v = constructComplexVector(1);

		pushOntoVector(&v, constructPointDynamically(3, 1));
		pushOntoVector(&v, constructPointDynamically(1, 1));
		pushOntoVector(&v, constructPointDynamically(4, 1));
		assert(((Point*)(v.data[0]))->x == 3);
		assert(((Point*)(accessVector(&v, 0)))->x == 3);
		assert(((Point*)(v.data[0]))->y == 1);
		assert(((Point*)(v.data[1]))->x == 1);
		assert(((Point*)(v.data[1]))->y == 1);
		assert(((Point*)(v.data[2]))->x == 4);
		assert(((Point*)(v.data[2]))->y == 1);
		destroyVector(&v);
	}
	printf("All tests passed\n");
}

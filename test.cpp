/* Contains Catch's main() function */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <unistd.h>
extern "C"{
#include "timer.h"
#include "vector.h"
#include "point.h"
}

// Get data from vector with index as int
long getLong(Vector* v, int index)
{
	return (long)v->data[index];
}
long getLastLong(Vector* v)
{
	return getLong(v, v->length - 1);
}


TEST_CASE("bla bla", "[bla]")
{
	SECTION("Make sure timer works Properly")
	{
		Timer t = constructTimer(500);
		Timer t2 = constructTimer(50000);
		REQUIRE(checkTimer(&t) == false);
		REQUIRE(checkTimer(&t2) == false);
		sleep(1);
		REQUIRE(checkTimer(&t));
		REQUIRE(checkTimer(&t) == false);
		REQUIRE(checkTimer(&t2) == false);
	}

	SECTION("Make sure vector works properly")
	{
		Vector v = constructSimpleVector(1);

		pushOntoVector(&v, (void*)3);
		REQUIRE(getLastLong(&v) == 3);

		pushOntoVector(&v, (void*)1);
		REQUIRE(getLastLong(&v) == 1);

		pushOntoVector(&v, (void*)5);
		REQUIRE(getLastLong(&v) == 5);

		popOffVector(&v);
		REQUIRE(getLastLong(&v) == 1);

		destroyVector(&v);
	}

	SECTION("Make sure points work vector")
	{
		Vector v = constructComplexVector(1);

		pushOntoVector(&v, constructPointDynamically(3, 1));
		pushOntoVector(&v, constructPointDynamically(1, 1));
		pushOntoVector(&v, constructPointDynamically(4, 1));
		REQUIRE(((Point*)(v.data[0]))->x == 3);
		REQUIRE(((Point*)(accessVector(&v, 0)))->x == 3);
		REQUIRE(((Point*)(v.data[0]))->y == 1);
		REQUIRE(((Point*)(v.data[1]))->x == 1);
		REQUIRE(((Point*)(v.data[1]))->y == 1);
		REQUIRE(((Point*)(v.data[2]))->x == 4);
		REQUIRE(((Point*)(v.data[2]))->y == 1);
		destroyVector(&v);
	}

}

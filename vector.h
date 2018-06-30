#pragma once
#include <stdbool.h>
typedef struct {
	int length;// Conceptual length of vector
	int physical_length; // Physical length of vector
	int expanse_rate; // By how many slots we expand when length exceeds physical length
	bool cleanup_data; // Do we cleanup individual data? (ie call free on each member of data)
	void** data;
} Vector;

// Create vector that doesn't clean up other than free()ing `data`
// Start with a physical length of expanse_rate
Vector constructSimpleVector(int expanse_rate);

// Vector in which the data are PROPER pointers instead of generic data
Vector constructComplexVector(int expanse_rate);

// Push data onto vector. Expand if needed
void pushOntoVector(Vector* v, void* datum);

// Same thing, except in front
void pushInFrontOfVector(Vector* v, void* datum);

// Remove particular item from vector
void removeFromVector(Vector* v, int index);

// Pop last member
void popOffVector(Vector* v);

// Access vector at index
void* accessVector(Vector* v, int index);

// Perform any cleanup needed
void destroyVector(Vector* v);

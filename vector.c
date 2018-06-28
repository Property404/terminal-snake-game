#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

static inline void errorIf(bool condition, const char* msg)
{
	if(condition)
	{
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

static Vector constructVector(int expanse_rate, bool cleanup_data)
{
	Vector v =
	{
		.length = 0,
		.physical_length = expanse_rate,
		.expanse_rate = expanse_rate,
		.cleanup_data = cleanup_data,
		.data = malloc(expanse_rate * sizeof(void*))
	}; 
	errorIf(v.data == NULL,
			"Allocation error in constructVector");
	return v;
}
Vector constructSimpleVector(int expanse_rate)
{
	return constructVector(expanse_rate, false);
}
Vector constructComplexVector(int expanse_rate)
{
	return constructVector(expanse_rate, true);
}

static void expandVector(Vector* v)
{
	v->physical_length += v->expanse_rate;
	void* temp = realloc(v->data, v->physical_length * sizeof(void*));

	errorIf(temp==NULL, "Reallocation error in expandVector()");

	v->data = temp;
}

void pushInFrontOfVector(Vector* v, void* datum)
{
	if(v->length >= v->physical_length)
		expandVector(v);

	for(size_t i=v->length; i>0; i--)
	{
		v->data[i] = v->data[i - 1];
	}
	v->data[0] = datum;

	v->length ++;
}


void pushOntoVector(Vector* v, void* datum)
{
	// Expand vector if necessary
	if(v->length >= v->physical_length)
		expandVector(v);

	/// And push
	v->data[v->length] = datum;
	v->length++;
}

void popOffVector(Vector* v)
{
	v->length--;
	errorIf(v->length < 0, "Out of bounds in popOffVector()");
	if(v->cleanup_data)
		free(v->data[v->length]);
}

void* accessVector(Vector* v, int index)
{
	errorIf(index<0 || index >= v->length,
			"Out of bounds in accessVector");
	return v->data[index];
}

void destroyVector(Vector* v)
{
	if(v->cleanup_data)
		for(int i = 0; i < (v->length); i++)
			free(v->data[i]);
	free(v->data);
}

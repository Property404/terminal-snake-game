#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
void errorIf(bool condition, const
		char* format, ...)
{
	if(condition)
	{
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
		fputc('\n', stderr);
		exit(1);
	}
}

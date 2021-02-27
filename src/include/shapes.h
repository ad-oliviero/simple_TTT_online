#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "raylib.h"
#endif

void place();
void shape(Rectangle*, int*, int*);

#include "Defs.h"

MoveDirection oppDir(MoveDirection dir) {
	if (dir == DIR_UP)
		return DIR_DOWN;
	if (dir == DIR_DOWN)
		return DIR_UP;
	if (dir == DIR_LEFT)
		return DIR_RIGHT;
	if (dir == DIR_RIGHT)
		return DIR_LEFT;
	return DIR_NONE;
}
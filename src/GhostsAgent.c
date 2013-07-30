
#include <stdio.h>
#include <stdlib.h>
#include "GhostsAgent.h"
#include "PacModel.h"

MoveDirection* rand_ghosts_dirs(GameState *gs) {
	static MoveDirection dirs[4];
	for (int i = 0; i < 4; i++) {		
		int index = arc4random() % gs->ghost_moves_size[i];
		MoveDirection * ghostMoves = gs->ghost_moves[i];
		dirs[i] = ghostMoves[index];
	}
	return dirs;
}

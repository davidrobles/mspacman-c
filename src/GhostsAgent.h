#ifndef _GHOSTSAGENT_
#define _GHOSTSAGENT_

#include "PacModel.h"
#include "Defs.h"

// Interface
extern MoveDirection* (*ghosts_directions)(GameState *gs);

// Implementations
MoveDirection* rand_ghosts_dirs(GameState *gs);

#endif _GHOSTSAGENT_
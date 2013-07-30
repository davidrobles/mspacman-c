#ifndef _PACMANAGENT_
#define _PACMANAGENT_

#include "Defs.h"
#include "PacModel.h"

// Interface
extern MoveDirection (*pacman_direction)(GameState *gs);

//Implementations
MoveDirection random_direction(GameState *gs);
MoveDirection nearest_pill(GameState *gs);

#endif _PACMANAGENT_
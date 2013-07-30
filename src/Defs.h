#ifndef _DEFS_
#define _DEFS_

typedef enum {
	DIR_UP,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_DOWN,
	DIR_NONE
} MoveDirection;

typedef enum {
	BLINKY,
	PINKY,
	INKY,
	SUE
} GhostType;

// MODEL
#define GHOSTS_SIZE 4

// POINTS
#define kPillPoints 10
#define kPowerPillPoints 10

// VIEW
#define kWidth  109
#define kHeight 121
#define KPacmanSize 12
#define GHOST_SIZE 12

#define kNodeSize 3
#define kPillSize 4
#define POWER_PILL_SIZE 8
#define START_LIVES 2
#define GHOST_COLLISION_DISTANCE 3
#define GAME_BG [NSColor blackColor]
#define kBlinkyColor 0, 1, 1, 1
#define kPinkyColor  1, 0.078, 0.576, 1
#define kInkyColor   1, 0, 0, 1
#define kSueColor    1, 0.549, 0, 1
#define kEdibleColor 0, 0, 1, 1

MoveDirection oppDir(MoveDirection dir);

#endif
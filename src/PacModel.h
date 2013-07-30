#ifndef _PACMODEL_
#define _PACMODEL_

#include "Defs.h"

typedef struct Node {
	int x;
	int y;
	int node_index;
	int pill_index;
	int power_index;
	struct Node *adj[4];
	int adjSize;
} Node;

typedef struct Maze {
	Node nodes[kWidth][kHeight];
	Node **path_nodes;
	int path_nodes_size;
	Node **pill_nodes;
	int pill_nodes_size;
	Node **power_nodes;	
	int power_pill_nodes_size;
	int distances[10000][10000];
	Node *pacman_start_node;
	Node *ghosts_start_node;	
} Maze;

typedef struct MazeState {
	Maze *maze;
	int pills_set[500];
	int powers_set[10];
	int pills_remaining;       // caching
	int power_pills_remaining; // caching
} MazeState;

typedef struct Pacman {
	struct Node *previous;
	struct Node *current;
	MoveDirection direction;
} Pacman;

typedef struct Ghost {
	struct Node *previous;
	struct Node *current;
	MoveDirection direction;
	GhostType type;
	int edible_time;
} Ghost;

typedef struct GameState {
	MazeState maze_state;
	Pacman pacman;
	Ghost ghosts[GHOSTS_SIZE];
	int score;
	int lives;
	int time_steps;
	int pacman_moves_total;
	MoveDirection pacman_moves[4];
	int ghost_moves_size[GHOSTS_SIZE];
	MoveDirection ghost_moves[GHOSTS_SIZE][4];	
} GameState;

// initialise the game state to default setup
void init_game_state(GameState *gs);

// initialise all the nodes of the maze
void init_maze_nodes(Maze *maze);

// load a maze from a file into the specified maze struct
void load_map(Maze *maze, char *map_file);

// calculate the adjacent path nodes for each path node of the maze
void set_adj(Maze *maze);

// initalises the four playable mazes
void init_mazes();

// moves
static void calculate_pacman_moves(GameState *gs);
static void calculate_ghosts_moves(GameState *gs);
static void move_pacman(GameState *gs, MoveDirection dir);
static void move_ghosts(GameState *gs, MoveDirection* dirs);

// game control
int is_game_over(GameState *gs);
static void eat_pills(GameState *gs);
static void check_maze_completion(GameState *gs);
void next_state(GameState *gs);
static int pacman_collided_with_ghosts(GameState *gs);
static void set_ghosts_edible(GameState *gs);
static void update_edible_times(GameState *gs);
static void add_points(GameState *gs, int score_increment);
static void reverse_ghosts_directions(Ghost *ghosts);

// getters
GameState *get_game_state();

// for calculatine node distances in a maze
static void set_distances(Maze *maze);
static void set_max(int *a, int size);
static void search(Node **path_nodes, int *distances, int curNodeIndex, int curDist);

// reset game state
void reset_game_state();
void reset_pacman(GameState *gs);
void reset_ghosts(GameState *gs);
void reset_ghost(GameState *gs, Ghost *ghost);
void reset_pills(MazeState *ms);
void reset_power_pills(MazeState *ms);
void reset_maze_state(MazeState *ms);

// utility functions
MoveDirection dir_to_neighbor(Node *current, Node *next);
Node *next_node_from(Node *srcNode, MoveDirection dir);

#endif _PACMODEL_
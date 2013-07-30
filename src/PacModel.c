#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PacModel.h"
#include "PacmanAgent.h"
#include "GhostsAgent.h"

Maze maze_a;
GameState game_state;

// Agents
MoveDirection (*pacman_direction)(GameState *gs) = nearest_pill;
MoveDirection* (*ghosts_directions)(GameState *gs) = rand_ghosts_dirs;

Node *goal_node;

void set_goal_node(Node *n) {
	goal_node = n;
}

Node *get_goal_node() {
	return goal_node;
}

void init_game_state(GameState *gs) {
	init_mazes();
	gs->maze_state.maze = &maze_a;
	gs->ghosts[0].type = BLINKY;
	gs->ghosts[1].type = PINKY;
	gs->ghosts[2].type = INKY;
	gs->ghosts[3].type = SUE;	
	reset_maze_state(&gs->maze_state);
	reset_game_state(gs);
}

void reset_game_state(GameState *gs) {
	gs->lives = START_LIVES;
	gs->score = 0;
	reset_pacman(gs);
	reset_ghosts(gs);
	reset_maze_state(&gs->maze_state);
}

void reset_pacman(GameState *gs) {
	gs->pacman.direction = DIR_LEFT;
	gs->pacman.previous = NULL;
	gs->pacman.current = gs->maze_state.maze->pacman_start_node;	
}

void reset_ghosts(GameState *gs) {
	for (int i = 0; i < GHOSTS_SIZE; i++)
		reset_ghost(gs, &gs->ghosts[i]);
}

void reset_ghost(GameState *gs, Ghost *ghost) {
	ghost->current = gs->maze_state.maze->ghosts_start_node;
	ghost->edible_time = 0;
}

void reset_maze_state(MazeState *ms) {
	reset_pills(ms);
	reset_power_pills(ms);
	ms->pills_remaining = ms->maze->pill_nodes_size;
	ms->power_pills_remaining = ms->maze->power_pill_nodes_size;		
}

void reset_pills(MazeState *ms) {
	for (int i = 0; i < ms->maze->pill_nodes_size; i++) {
		ms->pills_set[i] = 1;
	}	
}

void reset_power_pills(MazeState *ms) {
	for (int i = 0; i < ms->maze->power_pill_nodes_size; i++) {
		ms->powers_set[i] = 1;
	}		
}

void init_mazes() {
	init_maze_nodes(&maze_a);
	load_map(&maze_a, "/Users/drobles/Desktop/maps/A.txt");
	set_adj(&maze_a);
	set_distances(&maze_a);
}

static void set_distances(Maze *maze) {
	//nodeDistances = new int[nodes.size()][nodes.size()];
	for (int i = 0; i < maze->path_nodes_size; i++) {		
		set_max(maze->distances[i], maze->path_nodes_size);		
		search(maze->path_nodes, maze->distances[i], i, 0);
	}
}

static void set_max(int *a, int size) {
	for (int i = 0; i < size; i++) {
		a[i] = 99999999;
	}
}

static void search(Node **path_nodes, int *distances, int curNodeIndex, int curDist) {
	if (curDist < distances[curNodeIndex]) {
		distances[curNodeIndex] = curDist;
		for (int i = 0; i < path_nodes[curNodeIndex]->adjSize; i++) {			
			search(path_nodes, distances, path_nodes[curNodeIndex]->adj[i]->node_index, curDist + 1);
		}
	}
}

void init_maze_nodes(Maze *maze) {
	for (int x = 0; x < kWidth; x++) {
		for (int y = 0; y < kHeight; y++) {
			maze->nodes[x][y].x = x;
			maze->nodes[x][y].y = y;
			maze->nodes[x][y].adjSize = 0;
		}
	}		
}

void load_map(Maze *maze, char *map_file)
{
	FILE* fp;	
	fp = fopen(map_file, "r");
	char strMaze[kWidth][kHeight];
	char strBuffer[kWidth];
	int line = 0;
	
	while(fgets(strBuffer, kWidth + 2, fp) != NULL) {
		for (int x = 0; x < kWidth; x++)
			strMaze[x][line] = strBuffer[x];
		line++;
	}	
	
	for (int x = 0; x < kWidth; x++) {
		for (int y = 0; y < kHeight; y++) {
			switch (strMaze[x][y]) {
				// wall
				case '#':
					break;
				// pill
				case 'o':
					maze->path_nodes_size++;
					maze->pill_nodes_size++;
					break;
				// power pill
				case 'X':
					maze->path_nodes_size++;
					maze->power_pill_nodes_size++;
					break;
				// empty node
				case ' ':
					maze->path_nodes_size++;
					break;
				case 'G':
					maze->path_nodes_size++;
					break;
				case 'P':
					maze->path_nodes_size++;
					break;
				default:
					//printf("SOMETHING MUST BE WRONG! -> %c\n", strMaze[x][y]);
					break;				
			}
		}
	}
	
	maze->path_nodes = (Node**) malloc(maze->path_nodes_size * sizeof(Node*));
	maze->pill_nodes = (Node**) malloc(maze->pill_nodes_size * sizeof(Node*));
	maze->power_nodes = (Node**) malloc(maze->power_pill_nodes_size * sizeof(Node*));
	
	if (maze->nodes == NULL) {
		printf("valio burger");
	}
	
	int node_count = 0;
	int pill_count = 0;
	int powers_count = 0;
	
	for (int y = 0; y < kHeight; y++) {
		for (int x = 0; x < kWidth; x++) {
			switch (strMaze[x][y]) {
				// wall
				case '#':
					maze->nodes[x][y].adjSize = -1;
					maze->nodes[x][y].node_index = -1;					
					maze->nodes[x][y].pill_index = -1;
					maze->nodes[x][y].power_index = -1;
					break;
				// pill
				case 'o':
					maze->nodes[x][y].node_index = node_count;
					maze->nodes[x][y].pill_index = pill_count;
					maze->nodes[x][y].power_index = -1;
					maze->path_nodes[node_count++] = &maze->nodes[x][y];
					maze->pill_nodes[pill_count++] = &maze->nodes[x][y];
					break;
				// power pill
				case 'X':
					maze->nodes[x][y].node_index = node_count;
					maze->nodes[x][y].pill_index = -1;
					maze->nodes[x][y].power_index = powers_count;
					maze->path_nodes[node_count++] = &maze->nodes[x][y];
					maze->power_nodes[powers_count++] = &maze->nodes[x][y];
					break;
				// empty node
				case ' ':
					maze->nodes[x][y].node_index = node_count;
					maze->nodes[x][y].pill_index = -1;
					maze->nodes[x][y].power_index = -1;
					maze->path_nodes[node_count++] = &maze->nodes[x][y];
					break;
				// pacman start node
				case 'P':
					maze->nodes[x][y].node_index = node_count;
					maze->nodes[x][y].pill_index = -1;
					maze->nodes[x][y].power_index = -1;
					maze->path_nodes[node_count++] = &maze->nodes[x][y];
					maze->pacman_start_node = &maze->nodes[x][y];
					break;
				// ghost start node
				case 'G':
					maze->nodes[x][y].node_index = node_count;
					maze->nodes[x][y].pill_index = -1;
					maze->nodes[x][y].power_index = -1;
					maze->path_nodes[node_count++] = &maze->nodes[x][y];
					maze->ghosts_start_node = &maze->nodes[x][y];
					break;
				default:
					printf("SASD FSAD FA SDF A\n");
					break;	
			}
		}
	}	
}

void set_adj(Maze *maze) {
	for (int x = 0; x < kWidth; x++) {
		for (int y = 0; y < kHeight; y++) {
			if (maze->nodes[x][y].adjSize != -1) {
				// Down
				if (y > 0 && maze->nodes[x][y + 1].adjSize != -1) {
					int *ix = &maze->nodes[x][y].adjSize;
					maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[x][y + 1];
				}
				// Top
				if (y < (kHeight - 1) && maze->nodes[x][y - 1].adjSize != -1) {
					int *ix = &maze->nodes[x][y].adjSize;
					maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[x][y - 1];
				}
				// Left
				if (x > 0 && maze->nodes[x - 1][y].adjSize != -1) {
					int *ix = &maze->nodes[x][y].adjSize;
					if (x == (109 - 1)) {
						maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[0][y];
					}
					maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[x - 1][y];
				}
				// Right
				if (x < (kWidth - 1) && maze->nodes[x + 1][y].adjSize != -1) {
					int *ix = &maze->nodes[x][y].adjSize;
					// Left tunnels
					if (x == 0) {
						maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[109-1][y];
					}
					maze->nodes[x][y].adj[(*ix)++] = &maze->nodes[x + 1][y];					
				}
			}
		}
	}	
}

GameState *get_game_state() {
	return &game_state;
}

void next_state(GameState *gs) {
	if (is_game_over(gs)) {
		printf("Game Over!\n");
		return;
	}
	update_edible_times(gs);
	calculate_pacman_moves(gs);
	calculate_ghosts_moves(gs);
	move_pacman(gs, (*pacman_direction)(gs));
	move_ghosts(gs, (*rand_ghosts_dirs)(gs));
	eat_pills(gs);
	pacman_collided_with_ghosts(gs);
	check_maze_completion(gs);
}

static void check_maze_completion(GameState *gs) {
	printf("PILLS REM: %d\n", gs->maze_state.pills_remaining);
	printf("POWER PILLS REM: %d\n", gs->maze_state.power_pills_remaining);
	if (gs->maze_state.pills_remaining == 0 && gs->maze_state.power_pills_remaining == 0) {
		reset_ghosts(gs);
		reset_pacman(gs);
		reset_maze_state(&gs->maze_state);
	}	
}

static void calculate_pacman_moves(GameState *gs) {
	gs->pacman_moves_total = 0;
	for (int i = 0; i < gs->pacman.current->adjSize; i++) {
		Node *next = gs->pacman.current->adj[i];
		MoveDirection dir = dir_to_neighbor(gs->pacman.current, next);
		gs->pacman_moves[i] = dir;
		gs->pacman_moves_total++;
	}
}

static void calculate_ghosts_moves(GameState *gs) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {		
		gs->ghost_moves_size[i] = 0;		
		for (int j = 0; j < gs->ghosts[i].current->adjSize; j++) {
			struct Node *next = gs->ghosts[i].current->adj[j];
			MoveDirection dir = dir_to_neighbor(gs->ghosts[i].current, next);
			if (dir != oppDir(gs->ghosts[i].direction)) {
				gs->ghost_moves[i][gs->ghost_moves_size[i]++] = dir;
			}
		}
	}
}

static void move_pacman(GameState *gs, MoveDirection dir)
{	
	MoveDirection moveDir = DIR_NONE;
	int flag = 0;
	
	for (int i = 0; i < gs->pacman_moves_total; i++) {
		if (gs->pacman_moves[i] == dir) {
			flag = 1;
			break;
		}
	}
	
	int flag2 = 0;

	for (int i = 0; i < gs->pacman_moves_total; i++) {
		if (gs->pacman_moves[i] == gs->pacman.direction) {
			flag2 = 1;
			break;
		}
	}
	
	if (flag == 1) {
		moveDir = dir;
	} else if (flag2 == 1) {
		moveDir = gs->pacman.direction;
	}
	
	if (moveDir != DIR_NONE) {
		Node *next = next_node_from(gs->pacman.current, moveDir);
		gs->pacman.current = next;
		gs->pacman.direction = moveDir;
	}
}

static void move_ghosts(GameState *gs, MoveDirection* dirs) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {
		MoveDirection moveDir = DIR_NONE;
		int flag = 0;		
		for (int j = 0; j < gs->ghost_moves_size[i]; j++) {			
			if (gs->ghost_moves[i][j] == dirs[i]) {
				flag = 1;
				break;
			}
		}
		int flag2 = 0;		
		for (int j = 0; j < gs->ghost_moves_size[i]; j++) {
			if (gs->ghost_moves[i][j] == gs->ghosts[i].direction) {
				flag2 = 1;
				break;
			}
		}		
		if (flag == 1) {
			moveDir = dirs[i];
		} else if (flag2 == 1) {
			moveDir = gs->ghosts[i].direction;
		}		
		if (moveDir != DIR_NONE) {
			Node *next = next_node_from(gs->ghosts[i].current, moveDir);
			gs->ghosts[i].current = next;
			gs->ghosts[i].direction = moveDir;
		} else {
			printf("something is wrong here!!!\n");
		}
	}		
}

void eat_pills(GameState *gs) {
	if (gs->pacman.current->pill_index >= 0
		&& gs->maze_state.pills_set[gs->pacman.current->pill_index] == 1) {
		gs->maze_state.pills_set[gs->pacman.current->pill_index] = 0;
		add_points(gs, kPillPoints);
	} else if (gs->pacman.current->power_index >= 0
			   && gs->maze_state.powers_set[gs->pacman.current->power_index] == 1) {
		gs->maze_state.powers_set[gs->pacman.current->power_index] = 0;
		add_points(gs, kPowerPillPoints);
		reverse_ghosts_directions(gs->ghosts);
		set_ghosts_edible(gs);
	}
}

MoveDirection dir_to_neighbor(Node *current, Node *next) {
	// Up
	if (next->y < current->y)
		return DIR_UP;
	// Down
	else if (next->y > current->y)
		return DIR_DOWN;
	// Right
	else if (next->x > current->x) {
		// Left Tunnel
		if (next->x > current->x + 1)
			return DIR_LEFT;
		return DIR_RIGHT;
	}
	// Left
	else if (next->x < current->x) {
		// RightTunnel
		if (next->x < current->x - 1)
			return DIR_RIGHT;
		return DIR_LEFT;
	}
	return DIR_NONE;
}

Node *next_node_from(Node *srcNode, MoveDirection dir) {	
	for (int i = 0; i < srcNode->adjSize; i++) {
		struct Node *dstNode = srcNode->adj[i];
		if (dir_to_neighbor(srcNode, dstNode) == dir) {
			return dstNode;
		}
	}
	return NULL;
}

static int pacman_collided_with_ghosts(GameState *gs) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {
		int pic_ix = gs->pacman.current->node_index;
		int ghost_ix = gs->ghosts[i].current->node_index;
		int dist_to_ghost = gs->maze_state.maze->distances[pic_ix][ghost_ix];
		if (dist_to_ghost < GHOST_COLLISION_DISTANCE) {
			// eat the ghost
			if (gs->ghosts[i].edible_time > 0) {
				reset_ghost(gs, &gs->ghosts[i]);
			}
			// eaten by a ghost
			else {
				gs->lives--;
				reset_pacman(gs);
				reset_ghosts(gs);
			}
			return 1;
		}
	}
	return 0;
}

static void set_ghosts_edible(GameState *gs) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {
		gs->ghosts[i].edible_time = 150;
	}
}

static void update_edible_times(GameState *gs) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {
		gs->ghosts[i].edible_time--;
	}
}

int is_game_over(GameState *gs) {	
	return gs->lives < 0;
}

GameState copy_game_state(GameState *gs) {
	GameState new_gs;
	new_gs.lives = gs->lives;
	new_gs.score = gs->score;
	new_gs.ghost_moves;
	return new_gs;
}

void add_points(GameState *gs, int score_increment) {
	gs->score += score_increment;
}

static void reverse_ghosts_directions(Ghost *ghosts) {
	for (int i = 0; i < GHOSTS_SIZE; i++) {
		ghosts[i].direction = oppDir(ghosts[i].direction);
	}
}





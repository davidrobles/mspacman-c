
#include <stdio.h>
#include <stdlib.h>
#include "PacmanAgent.h"
#include "PacModel.h"
#include "Defs.h"

// Random move

MoveDirection random_direction(GameState *gs) {	
	MoveDirection dir = DIR_NONE;		
	while (dir == DIR_NONE) {
		int index = arc4random() % gs->pacman_moves_total;		
		MoveDirection tempDir = gs->pacman_moves[index];		
		if (tempDir != oppDir(gs->pacman.direction)) {
			dir = tempDir;
		}
	}
	return dir;
}

MoveDirection nearest_pill(GameState *gs) {
	Node *nearest_pill_node = NULL;
	int nearest_dist = 9999999;	
	for (int i = 0; i < gs->maze_state.maze->pill_nodes_size; i++) {
		if (gs->maze_state.pills_set[i] == 1) {
			int pac_ix = gs->pacman.current->node_index;
			int node_ix = gs->maze_state.maze->pill_nodes[i]->node_index;
			int dist = gs->maze_state.maze->distances[pac_ix][node_ix];
			if (dist < nearest_dist) {
				nearest_dist = dist;
				nearest_pill_node = gs->maze_state.maze->pill_nodes[i];
			}
		}
	}	
	if (nearest_pill_node == NULL)
		return random_direction(gs);		
	MoveDirection best_dir = DIR_NONE;
	int best_dir_value = 99999999;	
	for (int i = 0; i < gs->pacman_moves_total; i++) {
		Node *next_node = next_node_from(gs->pacman.current, gs->pacman_moves[i]);		
//		if (gs->pacman_moves[i] != oppDir(gs->pacman.direction)) {
			if (gs->maze_state.maze->distances[next_node->node_index][nearest_pill_node->node_index] < best_dir_value) {
				best_dir = gs->pacman_moves[i];
				best_dir_value = gs->maze_state.maze->distances[next_node->node_index][nearest_pill_node->node_index];
			}
//		}
	}
	return best_dir;
}



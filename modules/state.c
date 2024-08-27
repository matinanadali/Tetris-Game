#include "state.h"

///////////////////// General game data ///////////////////////

// Block shapes and rotations represented as 4x4 boolean grids
bool block_grids[][4][4][4] = {
	{	// type 0
		{	// rotation 0
			{0, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0}
		}
	},
		{	// type 1
		{	// rotation 0
			{0, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 0, 0, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 1, 0, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 0, 0, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 1, 0, 0}
		}
	},
		{	// type 2
		{	// rotation 0
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 0, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 1, 0, 0}
		},
		{	// rotation 3
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0}
		}
	},
		{	// type 3
		{	// rotation 0
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		}
	},
	{	// type 4
		{	// rotation 0
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 0}
		}
	},
	{	// type 5
		{	// rotation 0
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0}
		}
	},
	{	// type 6
		{	// rotation 0
			{0, 0, 0, 0},
			{0, 0, 0, 1},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		{	// rotation 1
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0}
		},
		{	// rotation 2
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{	// rotation 3
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0}
		}
	}

};

// The rightmost column index where the top-left corner of the block grid 
// can be positioned without any part of the block extending beyond the game grid's right edge
int max_scol[7] = {COLS-4, COLS-3, COLS-3, COLS-3, COLS-4, COLS-4,COLS-4};

// Block colors
Color colors[5] = {RED, GREEN, BLUE, YELLOW, VIOLET};

// Points awarded to the player for clearing rows.
// The array index represents the number of rows cleared simultaneously
const int rows_cleared_points[5] = {0, 40, 100, 300, 1200};

// Number of frames that must pass before the next move is applied.
// The array index represents the current game level
int frames_per_move[10] = {48,43,38,33,28,23,18,13,8,6}; 

//////////////////// Helper functions //////////////////////////

// Checks if the specified column x is within the bounds of the grid
bool in_grid(int x) {
	return 0 <= x && x < COLS;
}

// Generates a random integer between lower and upper (inclusive)
int irand(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

int* create_int(int value) {
	int *a = malloc(sizeof(int));
	*a = value;
	return a;
}

// Updates the block grid based on its type and orientation
void update_block_grid(Block block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block->grid[i][j] = block_grids[block->type][block->orientation][i][j];
        }
    }
}

//////////////////// foo_create functions //////////////////////////

// Creates and returns a new block
Block block_create() {
    Block block = malloc(sizeof(struct block));
	block->type = irand(0, 6);								// 7 block types
	block->position.x = irand(0, max_scol[block->type]);	// random starting column within the valid range
	// The first occupied cell of any block type with orientation = 0 
	// is located at the 3rd row of the 4x4 grid represanting the block. (see block_grids)
	// So, the top-left corner of the block grid, when it starts falling,
	// should be 3 cells above the top edge of the game grid 
	block->position.y = -3;									
	block->orientation = 0;
	block->color = colors[irand(0, 4)];						// random color
	update_block_grid(block);
	return block;
}

// Creates and returns initial state info
struct state_info state_info_create() {
    struct state_info info;
	info.paused = false;
	info.score = 0;
	info.game_over = false;
	info.moving_block = NULL;
	info.level = 0;
	info.rows_cleared_to_next_level = 10;		// level is updated after every 10 perfect row cleans
	return info;
}

// Creates and returns initial state events
struct state_events state_events_create() {
	struct state_events events;
	events.rows_cleared = 0;
	events.game_over = false;
	return events;
}

// Creates and returns initial state
State state_create() {
    State state = malloc(sizeof(*state));
	state->info = state_info_create();
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			state->occupied_cells[i][j] = EMPTY;
		}
	}
	state->next_block = block_create();
	state->events = state_events_create();
	state->frames_to_next_move = 0;
	return state;
}

//////////////////// Movement-related functions //////////////////////////

// Checks if the given "block" can move horizontally by "movement" cells (left if negative, right if positive).
bool valid_movement(Block block, int movement) {
	// Left- and right-most cell of block with current orientation
	int left_most, right_most;
	
	bool found_occupied_col = false;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			// Find first occupied column form left to right
			if (block->grid[row][col]) {
				found_occupied_col = true;
				left_most = block->position.x + col;
			}
		}
		if (found_occupied_col) break;
	}

	found_occupied_col = false;
	for (int col = 3; col >= 0; col--) {
		for (int row = 0; row < 4; row++) {
			// Find first occupied column from right to left
			if (block->grid[row][col]) {
				found_occupied_col = true;
				right_most = block->position.x + col;
			}
		}
		if (found_occupied_col) break;
	}

	// Check if the whole block is inside the grid when movement is applied
	return in_grid(left_most + movement) && in_grid(right_most + movement);
}

// Merges the moving block into the game grid by marking the cells covered by the block as MOVING.
void merge_block_grid(State state, Block block) {
	// Clear all cells previously marked as MOVING
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (state->occupied_cells[i][j] == MOVING)
				state->occupied_cells[i][j] = EMPTY;
		}
	}
	
	// Mark all cells that are currently covered by the moving block
	int srow = block->position.y;
	int scol = block->position.x;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block->grid[i][j]) 
				if(srow + i >= 0) {
					state->occupied_cells[srow + i][scol + j] = MOVING;
					state->cell_colors[srow + i][scol + j] = block->color;
				}
		}
	}
}

// Moves the block based on the key input
void move_block(State state, Block block, KeyState keys) {
    if (block == NULL) {
        return;
    }

    // Vertical movement
    block->position.y += 1;

	// Horizontal movement
	int movement = (keys->left ? -1 : (keys->right ? +1 : 0));
	if (valid_movement(block, movement)) {
		block->position.x += movement;
	}

	// Rotation
	if (keys->up) {
		block->orientation = (block->orientation + 1) % 4;
		update_block_grid(block);
	}

	merge_block_grid(state, block);
}

//////////////////// Landing-related functions //////////////////////////

// Checks if the moving block is landing
bool is_landing(State state) {
    for (int i = 0; i < ROWS-1; i++) {
		for (int j = 0; j < COLS; j++) {
			// Moving block lands on an occupied block below it
			if (state->occupied_cells[i][j] == MOVING && state->occupied_cells[i+1][j] == OCCUPIED) {
				return true;
			}
		}
	}
	for (int j = 0; j < COLS; j++) {
		// Moving block at the bottom of the grid
		if (state->occupied_cells[ROWS-1][j] == MOVING) {
			return true;
		}
	}
	return false;
}

// Returns a vector of all rows that are fully covered by blocks
Vector get_covered_rows(State state) {
	Vector covered_rows = vector_create(0, free);
	for (int i = 0; i < ROWS; i++) {
		bool covered = true;
		for (int j = 0; j < COLS; j++) {
			// Found a non-occupied block
			if (state->occupied_cells[i][j] != OCCUPIED) {
				covered = false;
				break;
			}
		}
		if (covered) vector_insert_last(covered_rows, create_int(i));
	}
	return covered_rows;
}

// Changes the state of the cells of the covered rows from occupied to cleared
void clear_covered_rows(State state) {
	Vector covered_rows = get_covered_rows(state);
	state->events.rows_cleared = vector_size(covered_rows);

	for (int i = 0; i < vector_size(covered_rows); i++) {
		for (int j = 0; j < COLS; j++) {
			// Clear line
			state->occupied_cells[*(int*)vector_get_at(covered_rows, i)][j] = CLEARED;
		}
	}
	vector_destroy(covered_rows);
}

// Marks the cells of the cleared rows as EMPTY, handles potential "falling" cells 
// and checks for new fully covered rows
void destroy_cleared_rows(State state) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (state->occupied_cells[i][j] == CLEARED) {
				state->occupied_cells[i][j] = EMPTY;
				// Occupied cells above the destroyed line are falling down
				int k = i;
				while (k >= 1 && state->occupied_cells[k-1][j] == OCCUPIED) {
					state->occupied_cells[k][j] = OCCUPIED;
					state->cell_colors[k][j] = state->cell_colors[k-1][j];
					state->occupied_cells[k-1][j] = EMPTY;
					k--;
				}
			}
		}
	}
	// Check again for covered rows
	clear_covered_rows(state);
}

// Handles the landing of a block
void handle_landing(State state) {
	// Change block state from moving to occupied
    for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (state->occupied_cells[i][j] == MOVING) {
				state->occupied_cells[i][j] = OCCUPIED;
				state->cell_colors[i][j] = state->info.moving_block->color;
			}
		}
	}
	
	// Check for fully covered rows
	clear_covered_rows(state);
	state->info.moving_block = NULL;
}

//////////////////// State_update-related functions //////////////////////////

// Generates a new moving block
void new_moving_block(State state) {
	// The block previously generated as next_block becomes the new moving block
	state->info.moving_block = state->next_block;
	merge_block_grid(state, state->info.moving_block);

	// Generate new next_block
	Block new_block = block_create();
	state->next_block = new_block;
}

// Returns whether the game is over
bool game_over(State state) {
	for (int j = 0; j < COLS; j++) {
		// Occupied cells have reached the top of the grid
		if (state->occupied_cells[0][j] == OCCUPIED) return true;
	}
	return false;
}

// Updates score, based on cleared rows and current level
void score_update(State state) {
	state->info.score += rows_cleared_points[state->events.rows_cleared] * state->info.level;
}

// Updates game level after every ten row are cleared
void level_update(State state) {
	state->info.rows_cleared_to_next_level -= state->events.rows_cleared;
	// Maximum level = 9
	if (state->info.rows_cleared_to_next_level <= 0 && state->info.level < 9) {
		state->info.level += 1;
	}
}

// Updates game state after each frame
void state_update(State state, KeyState keys) {
	if (state->frames_to_next_move > 0) {
		state->frames_to_next_move -= (keys->down ? 15 : 1);
		return;
	}

	if (state->events.rows_cleared != 0) {
		destroy_cleared_rows(state);
	}
	
	if (state->events.rows_cleared != 0) return;

	// Check for game over
	if (game_over(state)) {
		state->info.game_over = true;
	}
   	if (state->info.game_over == true) return;
	// Check for landing
	if (is_landing(state)) {
		handle_landing(state);
	}

	level_update(state);
	score_update(state);

	// Create new moving block
	if (state->info.moving_block == NULL) {
		new_moving_block(state);	
	}

   // Move block
   move_block(state, state->info.moving_block, keys);

   state->frames_to_next_move = frames_per_move[state->info.level];

}


// Destroys state and frees memory
void state_destroy(State state) {

}
#include "state.h"
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

int max_sx[7] = {COLS-4, COLS-3, COLS-3, COLS-3, COLS-4, COLS-4,COLS-4};

Color colors[5] = {RED, GREEN, BLUE, YELLOW, VIOLET};

bool in_grid(int x) {
	return 0 <= x && x < COLS;
}


// Generates a random integer between lower and upper (inclusive)
int irand(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

// Updates the block grid based on its type and orientation
void update_block_grid(Block block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block->grid[i][j] = block_grids[block->type][block->orientation][i][j];
        }
    }
}

// Creates and returns a new block
Block block_create() {
    Block block = malloc(sizeof(struct block));
	block->type = irand(0, 6);
	block->position.x = irand(0, max_sx[block->type]);
	block->position.y = -3;
	block->orientation = 0;
	block->color = colors[irand(0, 4)];
	update_block_grid(block);
	return block;
}

// Created and returns initial state info
struct state_info state_info_create() {
    struct state_info info;
	info.paused = false;
	info.score = 0;
	info.game_over = false;
	info.moving_block = NULL;
	return info;
}

// Creates and returns initial state
State state_create() {
    State state = malloc(sizeof(*state));
	state->blocks = vector_create(0, NULL);
	state->info = state_info_create();
	state->speed_factor = 1;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			state->occupied_cells[i][j] = EMPTY;
		}
	}
	return state;
}

// Returns basic game information
StateInfo state_info(State state) {
    return &state->info;
}

// Returns the vector of blocks
Vector state_blocks(State state) {
    return state->blocks;
}


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

void merge_block_grid(State state, Block block) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (state->occupied_cells[i][j] == MOVING)
				state->occupied_cells[i][j] = EMPTY;
		}
	}
	
	int srow = block->position.y;
	int scol = block->position.x;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block->grid[i][j]) 
				if(srow + i >= 0)
				state->occupied_cells[srow + i][scol + j] = MOVING;
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

int* create_int(int value) {
	int *a = malloc(sizeof(int));
	*a = value;
	return a;
}

Vector get_covered_rows(State state) {
	Vector covered_rows = vector_create(0, free);
	for (int i = 0; i < ROWS; i++) {
		bool covered = true;
		for (int j = 0; j < COLS; j++) {
			if (state->occupied_cells[i][j] != OCCUPIED) {
				covered = false;
				break;
			}
		}
		if (covered) vector_insert_last(covered_rows, create_int(i));
	}
	return covered_rows;
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

	// Check if occupied blocks are covering at least one whole row
	Vector covered_rows = get_covered_rows(state);
	while (vector_size(covered_rows) != 0) {
		for (int i = 0; i < vector_size(covered_rows); i++) {
			for (int j = 0; j < COLS; j++) {
				// Destroy line
				state->occupied_cells[*(int*)vector_get_at(covered_rows, i)][j] = EMPTY;
			}
		}
		// Occupied cells above the destroyed line are falling down
		int last_covered_row = *(int*)vector_get_at(covered_rows, vector_size(covered_rows)-1);
		for (int i = last_covered_row-1; i >= 0; i--) {
			for (int j = 0; j < COLS; j++) {
				if (state->occupied_cells[i][j] == OCCUPIED && state->occupied_cells[i+1][j] == EMPTY) {
					state->occupied_cells[i][j] = EMPTY;
					state->occupied_cells[i+1][j] = OCCUPIED;
					state->cell_colors[i+1][j] = state->cell_colors[i][j];
				}
			}
		}
		vector_destroy(covered_rows);
		covered_rows = get_covered_rows(state);
	}
	vector_destroy(covered_rows);
	state->info.moving_block = NULL;
}

void create_moving_block(State state) {
	Block new_block = block_create();
	state->info.moving_block = new_block;
	vector_insert_last(state->blocks, new_block);
	merge_block_grid(state, new_block);
}

bool game_over(State state) {
	for (int j = 0; j < COLS; j++) {
		if (state->occupied_cells[0][j] == OCCUPIED) return true;
	}
	return false;
}
// Updates game state after each frame
void state_update(State state, KeyState keys) {
	// Check for game over
   if (game_over(state)) {
		state->info.game_over = true;
   }
   if (state->info.game_over == true) return;
	// Check for landing
	if (is_landing(state)) {
		handle_landing(state);
	}

	// Create new moving block
	if (state->info.moving_block == NULL) {
		create_moving_block(state);	
	}

   // Move block
   move_block(state, state->info.moving_block, keys);

}


// Destroys state and frees memory
void state_destroy(State state) {

}
#pragma once

#include <stdlib.h>
#include "ADTVector.h"
#include "ADTList.h"
#include "vec2.h"
#include "math.h"
#include <stdio.h>
#include "raylib.h"
#include "ADTList.h"
#include "assert.h"

// Screen features
#define SCREEN_WIDTH 900	
#define SCREEN_HEIGHT 800

#define SIZE 40	// size of the side of each grid cell (pixels)
#define COLS 10
#define ROWS 20

extern int frames_per_move[10];

///////////////////////// Structure definitions ///////////////////////

// Cell states
typedef enum {EMPTY, OCCUPIED, MOVING, CLEARED} Cell;

typedef struct pair {
	int x;
	int y;
 } Pair;

 typedef struct block {
	int type;					// Type (= shape)
	Pair position;				// Position of top left corner
	int orientation;			// 0-3 clockwise 90-degree rotation
	bool grid[4][4];
	Color color;
}* Block;

// General game information
typedef struct state_info {
	Block moving_block;
	bool paused;
	bool game_over;					
	int score;
	int level;
	int rows_cleared_to_next_level;
					
}* StateInfo;

// Events happened at current state
typedef struct state_events {
	int rows_cleared;	// number of rows cleared
	bool game_over;
}* StateEvents;

typedef struct state {
    struct state_info info;	
	struct state_events events;
    Cell occupied_cells[ROWS][COLS];
	Color cell_colors[ROWS][COLS];
	Block next_block;
	int frames_to_next_move;	
}* State;

// Pressed buttons
typedef struct key_state {
	bool up;						// true if button is pressed
	bool left;
	bool right;
	bool down;
	bool enter;
	bool space;
}* KeyState;

//////////////////////// Function declaration //////////////////////////////////////////////

// Creates and returns initial state
State state_create();

// Updates game state after each frame
void state_update(State state, KeyState keys);

// Destroys state and frees memory
void state_destroy(State state);


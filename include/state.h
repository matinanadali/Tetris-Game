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

#define VERTICAL_SPEED 10    // pixels/frame
#define HORIZONTAL_SPEED 5 	
#define SIZE 40	
#define COLS 10
#define ROWS 20

/* Block types
	1. *
	   ****
	
	2. **
		**
	
	3.  *
	   ***

	4. **
	   **
	
	5.  **
	   **
	
	6. ****

	7.    *
	   ****
*/

typedef enum {EMPTY, OCCUPIED, MOVING} Cell;

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
}* StateInfo;



// Structure definitions
struct state {
    Vector blocks;			
    struct state_info info;	
    float speed_factor;		// 1 = regular game speed, 2 = double, etc
    Cell occupied_cells[ROWS][COLS];
	Color cell_colors[ROWS][COLS];
};




// Pressed buttons
typedef struct key_state {
	bool up;						// true if button is pressed
	bool left;
	bool right;
	bool enter;
	bool space;
}* KeyState;

// Game state
typedef struct state* State;

// Creates and returns initial state
State state_create();

// Returns basic game information
StateInfo state_info(State state);

// Returns a vector of all the visible blocks of the current state
Vector state_blocks(State state);

// Updates game state after each frame
void state_update(State state, KeyState keys);

// Destroys state and frees memory
void state_destroy(State state);


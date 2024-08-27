#include "raylib.h"
#include "state.h"
#include "ADTList.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>

#define X_OFFSET 0.3 * (SCREEN_WIDTH - SIZE * COLS)
#define COL1_WIDTH (X_OFFSET + COLS * SIZE)
#define COL2_WIDTH (SCREEN_WIDTH - COL1_WIDTH)

// Initializes game interface
void interface_init();

// Closes game interface
void interface_close();

// Draws a frame for the current game state
void interface_draw_frame(State state);
#include "interface.h"

void interface_init() {
	// Window initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(20);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}
float x_offset = 0.5 * (SCREEN_WIDTH - SIZE * COLS);
void draw_background() {
	
	// Draw vertical lines
	for (int i = 0; i <= COLS; i++) {
		DrawLine(x_offset + i * SIZE, 0, x_offset + i * SIZE, SCREEN_HEIGHT, WHITE); 
	}

	// Draw horizontal lines
	for (int i = 0; i < ROWS; i++) {
		DrawLine(x_offset, i * SIZE, x_offset + COLS * SIZE, i * SIZE, WHITE);
	}	
}

void draw_blocks(State state) {
	for (int k = 0; k < vector_size(state_blocks(state)); k++) {
		Block block = vector_get_at(state_blocks(state), k);
		int srow = block->position.y;
		int scol = block->position.x;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block->grid[i][j]) {
					DrawRectangle(x_offset + (scol + j) * SIZE, (srow + i) * SIZE, SIZE, SIZE, block->color);
					DrawRectangleLines(x_offset + (scol + j) * SIZE, (srow + i) * SIZE, SIZE, SIZE, WHITE);
				}
			}
		}
	}
}

void draw_game_over_screen() {
	DrawText("GAME OVER", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 20, 10, WHITE);
}
void interface_draw_frame(State state) {
    BeginDrawing();
	ClearBackground(BLACK);
	if (state_info(state)->game_over) {
		draw_game_over_screen();
	} else {
		draw_background();
		draw_blocks(state);
	}

	DrawFPS(SCREEN_WIDTH - 80, 0);
	
    EndDrawing();
}


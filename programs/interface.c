#include "interface.h"

void interface_init() {
	// Window initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(5);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}
const float x_offset = 0.3 * (SCREEN_WIDTH - SIZE * COLS);
const float next_block_offset = x_offset + COLS * SIZE + 0.3 * (SCREEN_WIDTH - COLS * SIZE - x_offset);
const float col1_width = x_offset + COLS * SIZE;
const float col2_width = SCREEN_WIDTH - col1_width;

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
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			if (state->occupied_cells[row][col] == CLEARED) {
				DrawRectangle(x_offset + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
				DrawRectangleLines(x_offset + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
			} else if (state->occupied_cells[row][col] != EMPTY) {
				DrawRectangle(x_offset + col * SIZE, row * SIZE, SIZE, SIZE, state->cell_colors[row][col]);
				DrawRectangleLines(x_offset + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
			}
		}
	}
}

void draw_next_block(State state) {
	int font_size = 40;
	int text_width = MeasureText("Next block\n", font_size);   
	DrawText("Next Block:\n", col1_width + 0.5 * (col2_width - text_width), 0.5 * SCREEN_HEIGHT, font_size, WHITE);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (state->next_block->grid[i][j]) {
				DrawRectangle(col1_width + 0.5 * (col2_width - 4 * SIZE) + j * SIZE, 0.5 * SCREEN_HEIGHT + i * SIZE, SIZE, SIZE, state->next_block->color);
				DrawRectangleLines(col1_width + 0.5 * (col2_width - 4 * SIZE)  + j * SIZE, 0.5 * SCREEN_HEIGHT + i * SIZE, SIZE, SIZE, WHITE);
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
		draw_next_block(state);
	}

	DrawFPS(SCREEN_WIDTH - 80, 0);
	
    EndDrawing();
}


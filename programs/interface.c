#include "interface.h"

void interface_init() {
	// Window initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(FONT_SIZE_BIG);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

void draw_background() {
	// Draw vertical lines
	for (int i = 0; i <= COLS; i++) {
		DrawLine(X_OFFSET + i * SIZE, 0, X_OFFSET + i * SIZE, SCREEN_HEIGHT, WHITE); 
	}

	// Draw horizontal lines
	for (int i = 0; i < ROWS; i++) {
		DrawLine(X_OFFSET, i * SIZE, X_OFFSET + COLS * SIZE, i * SIZE, WHITE);
	}	
}

void draw_blocks(State state) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			// Draw cleared cells (white squares)
			if (state->occupied_cells[row][col] == CLEARED) {
				DrawRectangle(X_OFFSET + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
				DrawRectangleLines(X_OFFSET + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
			} else if (state->occupied_cells[row][col] != EMPTY) {	// Draw occupied and moving squares
				DrawRectangle(X_OFFSET + col * SIZE, row * SIZE, SIZE, SIZE, state->cell_colors[row][col]);
				DrawRectangleLines(X_OFFSET + col * SIZE, row * SIZE, SIZE, SIZE, WHITE);
			}
		}
	}
}

void draw_next_block(State state) {
	int font_size = FONT_SIZE;
	int text_width = MeasureText("Next block\n", font_size);   
	DrawText("Next Block:\n", COL1_WIDTH + 0.5 * (COL2_WIDTH - text_width), 0.6 * SCREEN_HEIGHT, font_size, WHITE);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (state->next_block->grid[i][j]) {
				DrawRectangle(COL1_WIDTH + 0.5 * (COL2_WIDTH - 3 * SIZE) + j * SIZE, 0.7 * SCREEN_HEIGHT + i * SIZE, SIZE, SIZE, state->next_block->color);
				DrawRectangleLines(COL1_WIDTH + 0.5 * (COL2_WIDTH - 3 * SIZE)  + j * SIZE, 0.7 * SCREEN_HEIGHT + i * SIZE, SIZE, SIZE, WHITE);
			}
		}
	}
}

void draw_game_stats(State state) {
	int text_width = MeasureText("LEVEL:\n", FONT_SIZE);   
	DrawText("LEVEL:\n", COL1_WIDTH + 0.5 * (COL2_WIDTH - text_width), 0.2 * SCREEN_HEIGHT, FONT_SIZE, WHITE);
	text_width = MeasureText(TextFormat("%d\n", state->info.level), FONT_SIZE_BIG);
	DrawText(TextFormat("%d\n", state->info.level), COL1_WIDTH + 0.5 * (COL2_WIDTH - text_width), 0.3 * SCREEN_HEIGHT, FONT_SIZE, WHITE);
	text_width = MeasureText("SCORE:\n", FONT_SIZE);   
	DrawText("SCORE:\n", COL1_WIDTH + 0.5 * (COL2_WIDTH - text_width), 0.4 * SCREEN_HEIGHT, FONT_SIZE, WHITE);
	text_width = MeasureText(TextFormat("%d\n", state->info.score), FONT_SIZE_BIG);
	DrawText(TextFormat("%d\n", state->info.score), COL1_WIDTH + 0.5 * (COL2_WIDTH - text_width), 0.5 * SCREEN_HEIGHT, FONT_SIZE, WHITE);
}

void draw_game_over_screen() {
	int text_width = MeasureText("GAME OVER", FONT_SIZE_BIG);
	DrawText("GAME OVER", SCREEN_WIDTH * 0.5 - 0.5 * text_width, SCREEN_HEIGHT * 0.5 - 20, FONT_SIZE_BIG, WHITE);
	text_width = MeasureText("PRESS ENTER TO PLAY AGAIN", FONT_SIZE);
	DrawText("PRESS ENTER TO PLAY AGAIN", SCREEN_WIDTH * 0.5 - 0.5 * text_width, SCREEN_HEIGHT * 0.5 + 60, FONT_SIZE, WHITE);
}

void interface_draw_frame(State state) {
    BeginDrawing();
	ClearBackground(BLACK);
	if (state->info.game_over) {
		draw_game_over_screen();
	} else {
		draw_background();
		draw_blocks(state);
		draw_game_stats(state);
		draw_next_block(state);
	}

	DrawFPS(SCREEN_WIDTH - 80, 0);
	
    EndDrawing();
}


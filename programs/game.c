#include "game.h"
State state;
struct key_state keys;
bool left_pressed_last = false;	// stores whether the left or the right button is the last one pressed

void update_and_draw() {
	bool on_active_frame = state->frames_to_next_move == frames_per_move[state->info.level];

	if (IsKeyDown(KEY_LEFT)) left_pressed_last = true;
	else if (IsKeyDown(KEY_RIGHT)) left_pressed_last = false;

	keys = (struct key_state){
		// Keys might be pressed during non-active frames
		// In this case, we store that the key was pressed in order to apply the respective move on the next active frame
		// On each active frame, we ignore the previous information becaused the move is already applied
        IsKeyDown(KEY_UP) || (keys.up && !on_active_frame),
        IsKeyDown(KEY_LEFT) || (keys.left && !on_active_frame && left_pressed_last),
        IsKeyDown(KEY_RIGHT) || (keys.right && !on_active_frame && !left_pressed_last),
		IsKeyDown(KEY_DOWN),
        IsKeyDown(KEY_ENTER),
        IsKeyDown(KEY_SPACE),
    };
    
	state_update(state, &keys);
	interface_draw_frame(state);
}

int main() {
	interface_init();
	state = state_create();
	
	start_main_loop(update_and_draw);
    state_destroy(state);
	interface_close();
	return 0;
}
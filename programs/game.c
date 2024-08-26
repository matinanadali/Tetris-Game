#include "game.h"
State state;

void update_and_draw() {
	struct key_state keys = (struct key_state){
        IsKeyDown(KEY_UP),
        IsKeyDown(KEY_LEFT),
        IsKeyDown(KEY_RIGHT),
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
#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;

void update_and_draw() {
    // Αποθήκευση πατημένων πλήκτρων
    struct key_state keys = (struct key_state){
        IsKeyDown(KEY_UP),
        IsKeyDown(KEY_LEFT),
        IsKeyDown(KEY_RIGHT),
        IsKeyDown(KEY_ENTER),
        IsKeyDown(KEY_SPACE),
        IsKeyDown(KEY_N),
        IsKeyDown(KEY_P)
    };
    
	state_update(state, &keys);
	interface_draw_frame(state);
}

int main() {
	state = state_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);
    state_destroy(state);
	interface_close();
	return 0;
}
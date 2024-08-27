//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include <math.h>
#include "state.h"
#include "vec2.h"
#define INF 9999999999999L

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state->;
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

	
	TEST_ASSERT(info->moving_block == NULL);
	state_destroy(state);
}

void test_reaction_to_pressed_keys() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state-> != NULL);
	struct key_state keys = { false, false, false, false, false };
	// Update state to generate a moving block
	state_update(state, &keys);

	int block_x = state->moving_block->position.x;
	int block_y = state->moving_block->position.y;

	// Without any pressed keys, moving block only moves vertically
	state_update(state, &keys);
	int block_new_x = state->moving_block->position.x;
	int block_new_y = state->moving_block->position.y;
	TEST_ASSERT( block_new_x == block_x );
	TEST_ASSERT( block_new_y == block_y + 1 );

	// Αποδέσμευση μνήμης
	state_destroy(state);
}

void test_state_update() {
	test_reaction_to_pressed_keys();
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // Τερματίζουμε τη λίστα με NULL
};

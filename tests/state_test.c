//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"
#include "vec2.h"
#define INF 999999999

///// Βοηθητικές συναρτήσεις ////////////////////////////////////////
//
// Ελέγχει την (προσεγγιστική) ισότητα δύο double
// (λόγω λαθών το a == b δεν είναι ακριβές όταν συγκρίνουμε double).
static bool double_equal(double a, double b) {
	return abs(a-b) < 1e-6;
}

// Ελέγχει την ισότητα δύο διανυσμάτων
static bool vec2_equal(Vector2 a, Vector2 b) {
	return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}
/////////////////////////////////////////////////////////////////////


void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

	
	TEST_ASSERT(info->spaceship->type == SPACESHIP);
	//έλεγχος θέσης διαστημόπλοιου
	TEST_ASSERT(info->spaceship->position.x == 0 && info->spaceship->position.y == 0);

	//έλεγχος συνολικού αριθμού αστεροειδών - στην αρχική κατάσταση δεν υπάρχουν σφαίρες
	List initial_state_objects = state_objects(state, (Vector2){-INF, INF}, (Vector2){INF, -INF});
	TEST_ASSERT(list_size(initial_state_objects) == ASTEROID_NUM);
	
	//έλεγχος αριθμού αστεροειδών σε ορισμένο ορθογώνιο
	Vector2 top_left = {-2000, 9000};
	Vector2 bottom_right = {8000, -300};
	
	List limited_state_objects = state_objects(state, top_left, bottom_right);

	int number_of_asteroids_in_rectangle = 0;
	
	for (ListNode node = list_first(initial_state_objects); 
		node != LIST_EOF; 
		node = list_next(initial_state_objects, node)
		) {
		
		Vector2 position = *(Vector2*)list_node_value(initial_state_objects, node);
		if (top_left.x <= position.x &&
			top_left.y >= position.y &&
			bottom_right.x >= position.x &&
			bottom_right.y <= position.y)
		
			number_of_asteroids_in_rectangle++;
	}
	TEST_ASSERT(list_size(limited_state_objects) == number_of_asteroids_in_rectangle);
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το διαστημόπλοιο παραμένει σταθερό με μηδενική ταχύτητα
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,0}) );

	// Με πατημένο το πάνω βέλος, η ταχήτητα αυξάνεται ενώ το διαστημόπλοιο παραμένει για την ώρα ακίνητο
	keys.up = true;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,SPACESHIP_ACCELERATION}) );

	// Αν το πάνω βέλος δεν είναι πατημένο, το διαστημόπλοιο επιβραδύνεται
	//η ταχύτητά του δεν πρέπει να είναι ποτέ αρνητική
	keys.up = false;
	state_update(state, &keys);
	if (SPACESHIP_ACCELERATION-SPACESHIP_SLOWDOWN >= 0) {
		TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed, (Vector2){0,SPACESHIP_ACCELERATION-SPACESHIP_SLOWDOWN}) );
	} else {
		TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed, (Vector2){0,0}) );
	}

	// Με πατημένο το αριστερό βέλος, το διαστημόπλοιο περιστρέφεται κατά SPACESHIP_ROTATION αριστερόστροφα
	Vector2 initial_orientation = state_info(state)->spaceship->orientation;
	keys.left = true;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->orientation, vec2_rotate(initial_orientation, SPACESHIP_ROTATION)) );

	keys.left = false;

	// Έλεγχος λειτουργίας pause
	keys.p = true;
	state_update(state, &keys);

	TEST_ASSERT( state_info(state)->paused );
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};

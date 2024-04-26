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

///// Βοηθητικές συναρτήσεις ////////////////////////////////////////
//
// Ελέγχει την (προσεγγιστική) ισότητα δύο double
// (λόγω λαθών το a == b δεν είναι ακριβές όταν συγκρίνουμε double).
static bool double_equal(double a, double b) {
	return fabs(a-b) < 1e-6;
}

// Ελέγχει την ισότητα δύο διανυσμάτων
static bool vec2_equal(Vector2 a, Vector2 b) {
	return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}
static bool is_inside_rectangle(Vector2 top_left, Vector2 bottom_right, Vector2 position) {
	return top_left.x <= position.x &&
			top_left.y >= position.y &&
			bottom_right.x >= position.x &&
			bottom_right.y <= position.y;
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
	// 'Ελεγχος θέσης διαστημόπλοιου
	TEST_ASSERT(info->spaceship->position.x == 0 && info->spaceship->position.y == 0);

	// 'Ελεγχος συνολικού αριθμού αστεροειδών - στην αρχική κατάσταση δεν υπάρχουν σφαίρες
	List initial_state_objects = state_objects(state, (Vector2){-INF, INF}, (Vector2){INF, -INF});
	TEST_ASSERT(list_size(initial_state_objects) == ASTEROID_NUM);
	
	// 'Ελεγχος αριθμού αστεροειδών σε ορισμένο ορθογώνιο
	Vector2 top_left = {-2000, 9000};
	Vector2 bottom_right = {8000, -300};
	
	List limited_state_objects = state_objects(state, top_left, bottom_right);

	int number_of_asteroids_in_rectangle = 0;
	
	for (ListNode node = list_first(initial_state_objects); 
		node != LIST_EOF; 
		node = list_next(initial_state_objects, node)) 
	{
		Vector2 position = ((Object)list_node_value(initial_state_objects, node))->position;
		if (is_inside_rectangle(top_left, bottom_right, position)) {
			number_of_asteroids_in_rectangle++;
		}
	}
	TEST_ASSERT(list_size(limited_state_objects) == number_of_asteroids_in_rectangle);

	// Αποδέσμευση μνήμης
	list_destroy(limited_state_objects);
	list_destroy(initial_state_objects);
	state_destroy(state);
}

void test_reaction_to_pressed_keys() {
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

	
	keys.up = true;
	for (int i = 0; i < 100; i++) {
		state_update(state, &keys); // Το διαστημόπλοιο αποκτά μία αρχική θετική ταχυτήτα
	}

	double initial_speed = state_info(state)->spaceship->speed.y;
	// Αν το πάνω βέλος δεν είναι πατημένο, το διαστημόπλοιο επιβραδύνεται
	keys.up = false;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed, (Vector2){0,initial_speed-SPACESHIP_SLOWDOWN}) );
	
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

	// Αποδέσμευση μνήμης
	state_destroy(state);
}

void test_num_asteroids_update() {
	State state = state_create();
	// Πληροφορίες για τα πλήκτρα (κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };

	// Ελέγχουμε αν μετά από μερικές κλήσεις της state_update, ο αριθμός των αστεροειδών 
	// κοντά στο διαστημόπλοιο παράμενει τουλάχιστον ASTEROID_NUM
	for (int i = 0; i < 10; i++) {
		state_update(state, &keys);
	}

	// Συντεταγμένες διαστημοπλοίου
	double sx = state_info(state)->spaceship->position.x;
	double sy = state_info(state)->spaceship->position.y;

	Vector2 top_left = {sx-ASTEROID_MAX_DIST, sy+ASTEROID_MAX_DIST};
	Vector2 bottom_right = {sx+ASTEROID_MAX_DIST, sy-ASTEROID_MAX_DIST};

	List objects = state_objects(state, (Vector2){-INF, INF}, (Vector2){INF, -INF});
	int asteroids_near_spaceship = 0;

	for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
		Object object = list_node_value(objects, node);
		if (object->type == ASTEROID && is_inside_rectangle(top_left, bottom_right, object->position)) {
			asteroids_near_spaceship++;
		}
	}

	TEST_ASSERT(asteroids_near_spaceship >= ASTEROID_NUM);

	// Αποδέσμευση μνήμης
	list_destroy(objects);
	state_destroy(state);
}

void test_add_bullet() {
	State state = state_create();
	// Πληροφορίες για τα πλήκτρα 
	struct key_state keys = { true, true, false, false, false, false, false };

	// Το πάνω και το αριστερό βέλος είναι πατημένο - το διαστημόπλοιο στρίβει και μετατοπίζεται
	state_update(state, &keys);

	// To space είναι πατημένο - προστίθεται μία σφαίρα
	keys = (struct key_state){ false, false, false, false, true, false, false };
	state_update(state, &keys);

	Vector2 spaceship_orientation = state_info(state)->spaceship->orientation;
	Vector2 spaceship_speed = state_info(state)->spaceship->speed;
	Vector2 spaceship_position = state_info(state)->spaceship->position;

	Object bullet = NULL;
	List objects = state_objects(state,(Vector2){-INF, INF}, (Vector2){INF, -INF});
	for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
		Object object = list_node_value(objects, node);
		if (object->type == BULLET) {
			bullet = object;
			break;
		}
	}
	
	// Έλεγχος ότι υπάρχει σφαίρα
	TEST_ASSERT(bullet != NULL);
	// Η σφαίρα βρίσκεται στη θέση του διαστημοπλοίου
	TEST_ASSERT( vec2_equal(bullet->position, spaceship_position) );
	// Η σφαίρα έχει σχετική ταχύτητα BULLET_SPEED ως προς το διαστημόπλοιο
	TEST_ASSERT( vec2_equal(bullet->speed, vec2_add(spaceship_speed, vec2_scale(spaceship_orientation, BULLET_SPEED))));

	// Αποδέσμευση μνήμης
	list_destroy(objects);
	state_destroy(state);
}

void test_state_update() {
	test_reaction_to_pressed_keys();
	test_num_asteroids_update();
	test_add_bullet();
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // Τερματίζουμε τη λίστα με NULL
};

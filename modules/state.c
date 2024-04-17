
#include <stdlib.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (αστεροειδείς, σφαίρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	int next_bullet;		// Αριθμός frames μέχρι να επιτραπεί ξανά σφαίρα
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, Vector2 position, Vector2 speed, Vector2 orientation, double size) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->position = position;
	obj->speed = speed;
	obj->orientation = orientation;
	obj->size = size;
	return obj;
}

// Επιστρέφει έναν τυχαίο πραγματικό αριθμό στο διάστημα [min,max]

static double randf(double min, double max) {
	return min + (double)rand() / RAND_MAX * (max - min);
}

// Προσθέτει num αστεροειδείς στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα καρτεσιανό επίπεδο.
// - Η αρχή των αξόνων είναι η θέση του διαστημόπλοιου στην αρχή του παιχνιδιού
// - Στο άξονα x οι συντεταγμένες μεγαλώνουν προς τα δεξιά.
// - Στον άξονα y οι συντεταγμένες μεγαλώνουν προς τα πάνω.

static void add_asteroids(State state, int num) {
	for (int i = 0; i < num; i++) {
		// Τυχαία θέση σε απόσταση [ASTEROID_MIN_DIST, ASTEROID_MAX_DIST]
		// από το διστημόπλοιο.
		//
		Vector2 position = vec2_add(
			state->info.spaceship->position,
			vec2_from_polar(
				randf(ASTEROID_MIN_DIST, ASTEROID_MAX_DIST),	// απόσταση
				randf(0, 2*PI)									// κατεύθυνση
			)
		);

		// Τυχαία ταχύτητα στο διάστημα [ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED]
		// με τυχαία κατεύθυνση.
		//
		Vector2 speed = vec2_from_polar(
			randf(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * state->speed_factor,
			randf(0, 2*PI)
		);

		Object asteroid = create_object(
			ASTEROID,
			position,
			speed,
			(Vector2){0, 0},								// δεν χρησιμοποιείται για αστεροειδείς
			randf(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE)		// τυχαίο μέγεθος
		);
		vector_insert_last(state->objects, asteroid);
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.paused = false;				// Το παιχνίδι ξεκινάει χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->next_bullet = 0;					// Σφαίρα επιτρέπεται αμέσως
	state->info.score = 0;				// Αρχικό σκορ 0

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	state->objects = vector_create(0, NULL);

	// Δημιουργούμε το διαστημόπλοιο
	state->info.spaceship = create_object(
		SPACESHIP,
		(Vector2){0, 0},			// αρχική θέση στην αρχή των αξόνων
		(Vector2){0, 0},			// μηδενική αρχική ταχύτητα
		(Vector2){0, 1},			// κοιτάει προς τα πάνω
		SPACESHIP_SIZE				// μέγεθος
	);

	// Προσθήκη αρχικών αστεροειδών
	add_asteroids(state, ASTEROID_NUM);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

//ελέγχει αν το σημείο στη θέση position είναι εντός του ορθογωνίου που ορίζουν 
//τα σημεία top_left και bottom_right
bool is_inside_rectangle(Vector2 position, Vector2 top_left, Vector2 bottom_right) {
	return position.x >= top_left.x && 
		   position.x <= bottom_right.x &&
		   position.y <= top_left.y &&
		   position.y >= bottom_right.y;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.
List state_objects(State state, Vector2 top_left, Vector2 bottom_right) {
	List object_list = list_create(NULL);
	for (int i = 0; i < vector_size(state->objects); i++) {
		Object current_object = vector_get_at(state->objects, i);
		if (is_inside_rectangle(current_object->position, top_left, bottom_right)) 
			list_insert_next(object_list, list_last(object_list), current_object);
	}
	return object_list;
}

//ενημερώνει την κατάσταση του διαστημοπλοίου
void spaceship_update(Object spaceship, KeyState keys) {
	//περιστροφή διαστημοπλοίου
	if (keys->left || keys->right) {
		double rotation_angle = (keys->left ? 1 : -1) * SPACESHIP_ROTATION;
		spaceship->orientation = vec2_rotate(spaceship->orientation, rotation_angle);
	}
	
	if (keys->up) {  //επιτάχυνση διαστημοπλοίου
		Vector2 acceleration = vec2_scale(spaceship->orientation, SPACESHIP_ACCELERATION);
		spaceship->speed = vec2_add(spaceship->speed, acceleration);
	} else {        //επιβράδυνση διαστημοπλοίου
		Vector2 slowdown = vec2_scale(spaceship->orientation, -SPACESHIP_SLOWDOWN);
		spaceship->speed = vec2_add(spaceship->speed, slowdown);
		//το διαστημόπλοιο ακινητοποιείται
		if (spaceship->speed.x < 0 || spaceship->speed.y < 0) {
			spaceship->speed = (Vector2){0,0};
		}
	}
}

//ενημερώνει την κατάσταση του αντικειμένου (αστεροειδής-σφαίρα) object
void object_update(Object object, KeyState keys) {
	//ενημέρωση θέσης
	object->position = vec2_add(object->position, object->speed);
}

//ενημερώνει την κατάσταση του παιχνιδιού - paused / not paused
void game_update(State state, bool togglePause) {
	if (togglePause) {
		state->info.paused = !state->info.paused;
	} 
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.
void state_update(State state, KeyState keys) {
	game_update(state, keys->p);

	if (state_info(state)->paused) return;

	for (int i = 0; i < vector_size(state->objects); i++) {
		Object current_object = vector_get_at(state->objects, i);
		object_update(current_object, keys);
	}
	spaceship_update(state_info(state)->spaceship, keys);

	game_update(state, state->info.paused && keys->n);
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}


#include <stdlib.h>
#include "ADTSet.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"
#include "set_utils.h"
#include "math.h"
#include <stdio.h>

// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.
#define INF 9999999999L
struct state {
	Set asteroids;			// Περιέχει τους αστεροειδείς
	Set bullets; 			// Περιέχει τις σφαίρες
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
	int initial_size = set_size(state->asteroids);
	while(set_size(state->asteroids) < initial_size + num) {
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
		set_insert(state->asteroids, asteroid);
	}
}

int compare(Pointer a, Pointer b) {
    Object object_a = a;
    Object object_b = b;
    if (object_a->position.x < object_b->position.x) return 1;
    if (object_a->position.x > object_b->position.x) return -1;
    return 0;
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
	state->asteroids = set_create(compare, NULL);
	state->bullets = set_create(compare, NULL);

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

List state_objects_by_type(State state, Vector2 top_left, Vector2 bottom_right, ObjectType type) {
	Set objects_set = (type == BULLET) ? state->bullets : state->asteroids;
	List objects_list = list_create(NULL);
	SetNode starting_node = set_find_node_eq_or_greater(objects_set, &top_left.x);
    SetNode ending_node = set_find_node_eq_or_smaller(objects_set, &bottom_right.x);

    ending_node = (ending_node == SET_EOF) ? SET_EOF : set_next(objects_set, ending_node);
    for (SetNode node = starting_node; node != SET_EOF; node = set_next(objects_set, node)) {
        Object object = set_node_value(objects_set, node);
        if (object->position.y <= top_left.y && object->position.y >= bottom_right.y) {
            list_insert_next(objects_list, list_last(objects_list), object);
        }
    }
    return objects_list;
}

void list_append(List list, List to_append) {
	for (ListNode node = list_first(to_append); node != LIST_EOF; node = list_next(to_append, node)) {
		list_insert_next(list, list_last(list), list_node_value(to_append, node));
	}
	list_destroy(to_append);
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.
List state_objects(State state, Vector2 top_left, Vector2 bottom_right) {
	List objects = list_create(NULL);
	List asteroids = state_objects_by_type(state, top_left, bottom_right, ASTEROID);
	List bullets = state_objects_by_type(state, top_left, bottom_right, BULLET);
	list_append(objects, asteroids);
	list_append(objects, bullets);
	return objects;
}


// Ενημερώνει την κατάσταση του διαστημοπλοίου
void spaceship_update(Object spaceship, KeyState keys, double speed_factor) {
	//περιστροφή διαστημοπλοίου
	if (keys->left || keys->right) {
		double rotation_angle = (keys->left ? +1 : -1) * SPACESHIP_ROTATION;
		spaceship->orientation = vec2_rotate(spaceship->orientation, rotation_angle);
	}

	// Μετατόπιση διαστημοπλοίου
	spaceship->position = vec2_add(spaceship->position, vec2_scale(spaceship->speed, speed_factor));
	
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

// Ενημερώνει την κατάσταση του αντικειμένου (αστεροειδούς-σφαίρας) object
void object_update(Object object, int speed_factor) {
	// Μετατόπιση αντικειμένου
	object->position = vec2_add(object->position, vec2_scale(object->speed, speed_factor));
}

// Ενημερώνει την κατάσταση του παιχνιδιού - paused / not paused
void game_update(State state, bool togglePause) {
	if (togglePause) {
		state->info.paused = !state->info.paused;
	} 
}

// Ελέγχει αν ο αστεροειδής απέχει λιγότερο από ASTEROID_MAX_DIST από το διαστημόπλοιο
bool is_near_spaceship(Object object, Vector2 spaceship_position) {
	return vec2_distance(object->position, spaceship_position) < ASTEROID_MAX_DIST;
}

// Προσθέτει αστεροειδείς ώστε να υπάρχουν τουλάχιστον ASTEROID_NUM κοντά στο διαστημόπλοιο
void num_asteroids_update(State state) {

    Vector2 top_left = (Vector2){-ASTEROID_MAX_DIST + state->info.spaceship->position.x, +ASTEROID_MAX_DIST + state->info.spaceship->position.y};
	Vector2 bottom_right = (Vector2){+ASTEROID_MAX_DIST + state->info.spaceship->position.x, -ASTEROID_MAX_DIST + state->info.spaceship->position.y};
    List asteroids_near_spaceship = state_objects_by_type(state, top_left, bottom_right, ASTEROID);

	add_asteroids(state, ASTEROID_NUM - list_size(asteroids_near_spaceship));
    list_destroy(asteroids_near_spaceship);
}

// Προσθέτει μία σφαίρα, μόνο αν έχουν περάσει τουλάχιστον BULLET_DELAY frames από την προηγούμενη
void add_bullet(State state) {
	if (state->next_bullet > 0) return;
	Object spaceship = state->info.spaceship;
	
	// Η σφαίρα έχει σχετική ταχύτητα BULLET_SPEED ως προς το διαστημόπλοιο
	Vector2 speed = vec2_add(spaceship->speed, vec2_scale(spaceship->orientation, BULLET_SPEED));
	// Η σφαίρα αρχικά βρίσκεται στη θέση του διαστημοπλοίου
	Vector2 position = spaceship->position;

	set_insert(state->bullets, create_object(BULLET, position, speed, (Vector2){0, 0}, BULLET_SIZE));
	state->next_bullet = BULLET_DELAY;
}

// Ελέγχει αν τα αντικέιμενα a και b (θεωρούνται σφαιρικά κέντρου object->position και ακτίνας object->size) συγκρούονται
bool collapses(Object a, Object b) {
	return CheckCollisionCircles(a->position, a->size, b->position, b->size);
}

// Βοηθητική συνάρτηση για τη handle_collisions:
// Δημιουγεί έναν νέο αστεροειδή με συγκεκριμένη ταχύτητα και μέγεθος σε τυχαία απόσταση από το διαστημόπλοιο
Object create_new_asteroid(double size, double speed_value, Vector2 spaceship_position) {
	// Η ταχύτητα έχει ορισμένο μέτρο και τυχαία κατεύθυνση
	Vector2 speed = vec2_from_polar(
		speed_value,
		randf(0, 2*PI)
	);

	Vector2 position = vec2_add(
		spaceship_position,
		vec2_from_polar(
			randf(ASTEROID_MIN_DIST, ASTEROID_MAX_DIST),
			randf(0, 2*PI)									
		)
	);

	Object new_asteroid = create_object(
		ASTEROID,
		position,
		speed,
		(Vector2){0,0},
		size
	);

	return new_asteroid;
}

void handle_bullet_asteroid_collision(State state, Object asteroid) {
	double initial_size = asteroid->size;
	double initial_speed = vec2_distance((Vector2){0,0}, asteroid->speed);
	state->info.score -= 10;

	if (initial_size >= 2*ASTEROID_MIN_SIZE) {
		for (int i = 0; i < 2; i++) 
			set_insert(state->asteroids, create_new_asteroid(initial_size / 2, initial_speed * 1.5, state->info.spaceship->position));
		state->info.score += 2;
	}
	set_remove(state->asteroids, asteroid);
}

void handle_asteroid_spaceship_collision(State state, Object asteroid) {
	set_remove(state->asteroids, asteroid);
	state->info.score /= 2;
}

// Εντοπίζει και διαχειρίζεται τις συγκρούσεις αστεροειδούς-διαστημοπλοίου και αστεροειδούς-σφαίρας
void handle_collisions(State state) {
	List asteroids = state_objects_by_type(state, (Vector2){-INF, INF}, (Vector2){INF, -INF}, ASTEROID);
	List bullets = state_objects_by_type(state, (Vector2){-INF, INF}, (Vector2){INF, -INF}, BULLET);

	for (ListNode a = list_first(asteroids); a != LIST_EOF; a = list_next(asteroids, a)) {
		Object asteroid = list_node_value(asteroids, a);
		for (ListNode b = list_first(bullets); b != LIST_EOF; b = list_next(bullets, b)) {
			Object bullet = list_node_value(bullets, b); 
			if (collapses(asteroid, bullet)) {
				handle_bullet_asteroid_collision(state, asteroid);
			}
		}

		if (collapses(asteroid, state->info.spaceship)) {
			handle_asteroid_spaceship_collision(state, asteroid);
		}
	}
	list_destroy(asteroids);
	list_destroy(bullets);
}


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.
void state_update(State state, KeyState keys) {
	
	// Αλλάζει την κατάσταση του παιχνιδιού (paused - not paused) αν το p είναι πατημένο
	game_update(state, keys->p);

	if (state->info.paused) return;

	// Συγκρούσεις
//	handle_collisions(state);

	// Ενημέρωση κατάστασης αντικειμένων
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    Vector2 top_left = (Vector2){-2*width+ state->info.spaceship->position.x, +2*height + state->info.spaceship->position.y};
	Vector2 bottom_right = (Vector2){2*width + state->info.spaceship->position.x, -2*height + state->info.spaceship->position.y};
    List objects= state_objects(state, top_left, bottom_right);
	for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
		Object current_object = list_node_value(objects, node);
		object_update(current_object, state->speed_factor);
	}
    list_destroy(objects);

	// Ενημέρωση κατάστασης διαστημοπλοίου
	spaceship_update(state_info(state)->spaceship, keys, state->speed_factor);

	// Προσθήκη νέων αστεροειδών
	num_asteroids_update(state);
	
	// Μείωση του αριθμού frames που απαιτούνται για να επιτραπεί η επόμενη σφαίρα
	state->next_bullet--;

	// Προσθήκη νέας σφαίρας
	if (keys->space) {
		add_bullet(state);
	}

	// Αύξηση της ταχύτητας παιχνιδιού αν το σκορ φτάσει σε κάποιο πολλαπλάσιο του 100
	if (state != 0 && state->info.score % 100 == 0) {
		state->speed_factor *= 1.1;
	}
	game_update(state, state->info.paused && keys->n);
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη
void state_destroy(State state) {
	for (SetNode node = set_first(state->asteroids); node != SET_EOF; node = set_next(state->asteroids, node))  {
		free(set_node_value(state->asteroids, node));
	}
    set_destroy(state->asteroids);
	for (SetNode node = set_first(state->bullets); node != SET_EOF; node = set_next(state->bullets, node))  {
		free(set_node_value(state->bullets, node));
	}
    set_destroy(state->bullets);
	free(state->info.spaceship);
	free(state);
}

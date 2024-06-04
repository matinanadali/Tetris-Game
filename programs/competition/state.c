#include <stdlib.h>
#include "ADTVector.h"
#include "ADTList.h"
#include "state_competition.h"
#include "vec2.h"
#include "math.h"
#include <stdio.h>
#include <assert.h>

static void add_asteroids(State state, int num);
static void add_stars(State state, int num);
void destroy_object(Object object);

///// Βοηθητικές συναρτήσεις ////////////////////////////////////////////////////////////////////////////////////////////

// Επιστρέφει έναν τυχαίο πραγματικό αριθμό στο διάστημα [min,max]
static double randf(double min, double max) {
	return min + (double)rand() / RAND_MAX * (max - min);
}

// Aφαιρεί το στοιχείο στη pos του vector
void vector_remove_at(Vector vector, int pos) {
	if (pos == vector_size(vector)-1) {
		vector_remove_last(vector);
	} else {
		Object last_value = vector_get_at(vector, vector_size(vector)-1);
		Object value_to_remove = vector_get_at(vector, pos);

		// Ανταλλαγή στοιχείου προς διαγραφή με το τελευταίο στοιχείο του vector
		DestroyFunc destroy_value = vector_set_destroy_value(vector, NULL);
		vector_set_at(vector, pos, last_value);
		vector_set_at(vector, vector_size(vector)-1, value_to_remove);
		vector_set_destroy_value(vector, destroy_value);

		// Διαγραφή τελευταίου στοιχείου
		vector_remove_last(vector);
	}
}

// Eλέγχει αν το σημείο στη θέση position είναι εντός του ορθογωνίου που ορίζουν 
// τα σημεία top_left και bottom_right
bool is_inside_rectangle(Vector2 position, Vector2 top_left, Vector2 bottom_right) {
	return position.x >= top_left.x && 
		   position.x <= bottom_right.x &&
		   position.y <= top_left.y &&
		   position.y >= bottom_right.y;
}

// Βοηθητική συνάρτηση για τις συγκρούσεις - υπολογίζει την ακτίνα των αντικειμένων με βάση
// τον τρόπο που σχεδιάζονται
float get_object_radius(Object object) {
	float radius;
	if (object->type == ASTEROID) {
		float scale_ratio = object->size / ASTEROID_MAX_SIZE;
		// Ο μεγαλύτερος αστεροειδής έχει διάμετρο ASTEROID_WIDTH
		radius = scale_ratio * ASTEROID_WIDTH * 0.5;
	} else if (object->type == BULLET) {
		radius = object->size;
	} else {
		radius = SPACESHIP_SIZE * 0.5;
	}
	return radius;
}

// Ελέγχει αν τα αντικέιμενα a και b (θεωρούνται σφαιρικά κέντρου object->position και ακτίνας object->size) συγκρούονται
bool collide(Object a, Object b) {
	float a_radius = get_object_radius(a);
	float b_radius = get_object_radius(b);
	return CheckCollisionCircles(a->position, a_radius, b->position, b_radius);
}

///// Συναρτήσεις δημιουργίας ////////////////////////////////////////////////////////////////////////////////////////////


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (αστεροειδείς, σφαίρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	int next_bullet;		// Αριθμός frames μέχρι να επιτραπεί ξανά σφαίρα
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};

// Δημιουργεί και επιστρέφει ένα τυχαίο asteroid_state (τύπος αστεροειδούς, ταχύτητα περιστροφής και αρχική περιστροφή)
AsteroidState create_asteroid_state() {
	AsteroidState asteroid_state = malloc(sizeof(*asteroid_state));
	asteroid_state->type = rand() % 8; 	// 8 τύποι αστεροειδών για ποικιλία στη σχεδίαση
	asteroid_state->rotation = (Vector2){0,1};
	asteroid_state->rotation_speed = randf(ASTEROID_MIN_ROTATION_SPEED, ASTEROID_MAX_ROTATION_SPEED);
	return asteroid_state;
}


// Δημιουργεί και επιστρέφει ένα αντικείμενο
static Object create_object(ObjectType type, Vector2 position, Vector2 speed, Vector2 orientation, double size) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->asteroid_state = create_asteroid_state();
	obj->position = position;
	obj->speed = speed;
	obj->orientation = orientation;
	obj->size = size;
	return obj;
}

// Βοηθητική συνάρτηση για τη handle_collisions:
// Δημιουγεί έναν νέο αστεροειδή με συγκεκριμένη ταχύτητα, τύπο και μέγεθος 
Object create_new_asteroid(int size, double speed_value, double speed_angle, Vector2 position, int type) {
	Vector2 speed = vec2_from_polar(speed_value, speed_angle);

	Object new_asteroid = create_object(
		ASTEROID,
		position,
		speed,
		(Vector2){0,0},
		size
	);

	// Αλλάσουμε τον τυχαία ορισμένο τύπο που έδωσε η συνάρτηση create_object
	new_asteroid->asteroid_state->type = type;

	return new_asteroid;
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού
State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.paused = false;				// Το παιχνίδι ξεκινάει χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->next_bullet = 0;					// Σφαίρα επιτρέπεται αμέσως

	state->info.screen_state = malloc(sizeof(struct screen_state));
	state->info.screen_state->screen = WELCOME;							// Αρχική οθόνη
	state->info.screen_state->frames_in_transition = 0;					// Frames στη μεταβατική οθόνη

	state->info.events = malloc(sizeof(struct events));					// Γεγονότα στο τρέχον state
	state->info.events->bullet_thrown = false;							// Πέταγμα σφαίρας
	state->info.events->bullet_added = 0;								// Ο παίκτης κέρδισε μία σφαίρα
	state->info.events->star = false;									// Ο παίκτης κέρδισε ένα αστέρι
	state->info.events->collision = false;								// Σύγκρουση

	state->info.stats = malloc(sizeof(struct stats));					
	state->info.stats->score = 0;										// Σκορ
	state->info.stats->lives = INITIAL_LIVES;							// Ζωές παίκτη
	state->info.stats->bullets_left = INITIAL_BULLETS;					// Διαθέσιμες σφαίρες παίκτη

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	state->objects = vector_create(0, (DestroyFunc)destroy_object);

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
	add_stars(state, STAR_NUM);

	return state;
}

///// Συναρτήσεις πληροφοριών για το τρέχον state ///////////////////////////////////////////////////////////////////////////

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state
StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.
List state_objects(State state, Vector2 top_left, Vector2 bottom_right) {
	List object_list = list_create(NULL);
	
	for (int i = 0; i < vector_size(state->objects); i++) {
		Object current_object = vector_get_at(state->objects, i);
		if (is_inside_rectangle(current_object->position, top_left, bottom_right)) {
			list_insert_next(object_list, list_last(object_list), current_object);
		}
	}

	return object_list;
}

///// Συναρτήσεις προσθήκης αντικειμένων ///////////////////////////////////////////////////////////////////////////////////

// Προσθέτει num αστεροειδείς στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
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

// Προσθέτει num αστέρια
static void add_stars(State state, int num) {
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

		Vector2 speed = (Vector2){0,0};

		Object star = create_object(
			STAR,
			position,
			speed,
			(Vector2){0, 0},								// δεν χρησιμοποιείται για αστέρια
			STAR_SIZE
		);
		vector_insert_last(state->objects, star);
	}
}

// Προσθέτει μία σφαίρα, μόνο αν έχουν περάσει τουλάχιστον BULLET_DELAY frames από την προηγούμενη 
// και ο παίκτης έχει διαθέσιμες σφαίρες
void add_bullet(State state) {
	if (state->next_bullet > 0 || state_info(state)->stats->bullets_left == 0) return;
	
	state->info.events->bullet_thrown = true;
	Object spaceship = state->info.spaceship;
	
	// Η σφαίρα έχει σχετική ταχύτητα BULLET_SPEED ως προς το διαστημόπλοιο
	Vector2 speed = vec2_add(spaceship->speed, vec2_scale(spaceship->orientation, BULLET_SPEED));
	// Η σφαίρα αρχικά βρίσκεται στη θέση του διαστημοπλοίου
	Vector2 position = spaceship->position;
	// Η σφαίρα έχει την κατεύθυνση του διαστημοπλοίου
	Vector2 orientation = spaceship->orientation;
	vector_insert_last(state->objects, create_object(BULLET, position, speed, orientation, 0));

	state->next_bullet = BULLET_DELAY;
	state_info(state)->stats->bullets_left--;
}

///// Συναρτήσεις μεταβολής κατάστασης (update) ///////////////////////////////////////////////////////////////////////////////////

// Ελέγεχει αν ο παίκτης έχασε - αν οι ζωές του μηδενίστηκαν
bool game_over(State state) {
	if (state->info.stats->lives <= 0) {
		state->info.screen_state->screen = GAME_OVER;
		return true;
	}
	return false;
}


// Μεταβάλλει το σκορ του state κατά points πόντους
void score_update(State state, int points) {
	state->info.stats->score += points;
	// Για κάθε 10 πόντους, ο παίκτης κερδίζει μία σφαίρα με maximum τις 5
	if (state->info.stats->score != 0 && state->info.stats->score % 10 == 0) {
		if (state->info.stats->bullets_left < 5) {
			state->info.stats->bullets_left++;
			state->info.events->bullet_added = 1;
		}
	}
}

// Frames από την τελευταία φορά που ο παίκτης κέρδισε μία σφαίρα
// Χρησιμοποιείται για την εμφάνιση του +1 για ακριβώς ένα δευτερόλεπτο μετά
// από την προσθήκη κάθε σφαίρας
void frames_from_added_bullet_update(State state) {
	// Αν έχει προστεθεί σφαίρα πρόσφατα, αυξάνουμε τα frames 
	// από τη στιγμή της προσθήκης της
	if (state->info.events->bullet_added > 0) {
		state->info.events->bullet_added++;
	}
	// Αν πέρασαν 60 frames από την προσθήκη της σφαίρας, 
	// μηδενίζουμε τον μετρητή
	if (state->info.events->bullet_added > 60) {
		state->info.events->bullet_added = 0; 
	}
}

// Ενημερώνει την κατάσταση του διαστημοπλοίου
void spaceship_update(Object spaceship, KeyState keys, State state) {
	// Μετατόπιση διαστημοπλοίου
	spaceship->position = vec2_add(spaceship->position, vec2_scale(spaceship->speed, state->speed_factor));

	if (keys->up) {  // Eπιτάχυνση διαστημοπλοίου
		Vector2 acceleration = vec2_scale(spaceship->orientation, SPACESHIP_ACCELERATION);
		spaceship->speed = vec2_add(spaceship->speed, acceleration);
	} else {        // Eπιβράδυνση διαστημοπλοίου
		Vector2 initial_speed = state->info.spaceship->speed;
		Vector2 slowdown = vec2_scale(spaceship->speed, -SPACESHIP_SLOWDOWN);
		spaceship->speed = vec2_add(spaceship->speed, slowdown);

		// Η επιβράδυνση του διαστημοπλοίου μειώνει το μέτρο της ταχύτητας αλλά δεν πρέπει να αλλάζει την κατεύθυνσή της
		// Αν η νέα ταχύτητα δεν έχει την ίδια κατεύθυνση με την αρχική, μηδενίζεται
		if (!(initial_speed.x * spaceship->speed.x > 0 || initial_speed.y * spaceship->speed.y > 0)) {
			spaceship->speed = (Vector2){0,0};
		}
	}

	// Περιστροφή διαστημοπλοίου
	if (keys->left || keys->right) {
		double rotation_angle = (keys->left ? +1 : -1) * SPACESHIP_ROTATION;
		spaceship->orientation = vec2_rotate(spaceship->orientation, rotation_angle);
	}
}

// Ενημερώνει την κατάσταση του αντικειμένου object
void object_update(State state, Object object, int pos) {
	// Μετατόπιση αντικειμένου
	object->position = vec2_add(object->position, vec2_scale(object->speed, state->speed_factor));

	if (object->type == ASTEROID) {
		object->asteroid_state->rotation = vec2_rotate(object->asteroid_state->rotation, object->asteroid_state->rotation_speed);
	}
}

// Προσθέτει αστεροειδείς και αστέρια ώστε να υπάρχουν τουλάχιστον ASTEROID_NUM και STAR_NUM κοντά στο διαστημόπλοιο
void num_objects_update(State state) {
	int asteroids_near_spaceship = 0;
	int stars_near_spaceship = 0;
	
	// Συντεταγμένες διαστημοπλοίου
	double sx = state->info.spaceship->position.x;
	double sy = state->info.spaceship->position.y;

	// Λίστα αντικειμένων στο τετράγωνο με κέντρο το διαστημόπλοιο και πλευρά 2 ASTEROID_MAX_DIST 
	List objects_near_spaceship = state_objects(state, (Vector2){sx - ASTEROID_MAX_DIST, sy + ASTEROID_MAX_DIST},
												 	   (Vector2){sx + ASTEROID_MAX_DIST, sy - ASTEROID_MAX_DIST});
	
	for (ListNode node = list_first(objects_near_spaceship); node != LIST_EOF; node = list_next(objects_near_spaceship, node)) {
		if (((Object)list_node_value(objects_near_spaceship, node))->type == ASTEROID) {
			asteroids_near_spaceship++;
		} else if (((Object)list_node_value(objects_near_spaceship, node))->type == STAR) {
			stars_near_spaceship++;
		}
	}
	
	list_destroy(objects_near_spaceship);

	add_asteroids(state, ASTEROID_NUM - asteroids_near_spaceship);
	add_stars(state, STAR_NUM - stars_near_spaceship);
}

///// Συγκρούσεις //////////////////////////////////////////////////////////////////////////////////////////////////////////

// Διαχειρίζεται τις συγκρούσεις αστεροειδούς - σφαίρας
void handle_bullet_asteroid_collision(State state, Object bullet, Object asteroid, int pos) {
	// Αν το μέγεθος του αρχικού αστεροειδούς ήταν αρκετά μεγάλο, 
	// δημιουργούνται 2 νέοι με το μισό μέγεθος και ταχύτητα μέτρου 4 φορές μεγαλύτερη του αρχικού
	if (asteroid->size >= 2*ASTEROID_MIN_SIZE) {
		for (int i = 0; i < 2; i++) {
			double speed_value = vec2_distance(asteroid->speed, (Vector2){0,0}) * 4; // ταχύτητα τετραπλάσιου μέτρου

			Vector2 bullet_asteroid_vector = vec2_add(bullet->position, vec2_scale(asteroid->position, -1));

			double speed_angle = atan2(bullet_asteroid_vector.y, bullet_asteroid_vector.x);
			// Μετατροπή στο σύστημα συντεταγμένων
			speed_angle += PI / 2;
			// Προσθήκη μίας τυχαίας απόκλισης από τον άξονα σφαίρας-αστεροειδούς
			speed_angle += randf(ASTEROID_MIN_ANGLE_AFTER_COLLISION, ASTEROID_MAX_ANGLE_AFTER_COLLISION);
			
			vector_insert_last(state->objects, create_new_asteroid(asteroid->size * 0.5, 
																   speed_value, speed_angle, 
																   asteroid->position, 
																   asteroid->asteroid_state->type));	
		}
	}
	// Διαγραφή αρχικού αστεροειδή
	vector_remove_at(state->objects, pos);
}

// Διαχειρίζεται τη σύγκρουση αστεροειδή - διαστημοπλοίου
void handle_asteroid_spaceship_collision(State state, Object asteroid, int pos) {
	// Αν το μέγεθος του αρχικού αστεροειδούς ήταν αρκετά μεγάλο, 
	// δημιουργούνται 2 νέοι με το μισό μέγεθος και ταχύτητα μέτρου 4 φορές μεγαλύτερη του αρχικού
	if (asteroid->size >= 2*ASTEROID_MIN_SIZE) {
		for (int i = 0; i < 2; i++) {
			double speed_value = vec2_distance(asteroid->speed, (Vector2){0,0}) * 4; // ταχύτητα τετραπλάσιου μέτρου

			Vector2 spaceship_asteroid_vector = vec2_add(state->info.spaceship->position, vec2_scale(asteroid->position, -1));
			
			double speed_angle = atan2(spaceship_asteroid_vector.y, spaceship_asteroid_vector.x);
			// Μετατροπή στο σύστημα συντεταγμένων
			speed_angle += PI / 2;
			// Προσθήκη μίας τυχαίας απόκλισης από τον άξονα σφαίρας-αστεροειδούς
			speed_angle += randf(ASTEROID_MIN_ANGLE_AFTER_COLLISION, ASTEROID_MAX_ANGLE_AFTER_COLLISION);
			
			vector_insert_last(state->objects, create_new_asteroid(asteroid->size * 0.5, 
																   speed_value, speed_angle, 
																   asteroid->position, 
																   asteroid->asteroid_state->type));	
		}
	}

	// Διαγραφή αρχικού αστεροειδή
	vector_remove_at(state->objects, pos);

	// Ο παίκτης χάνει μία ζωή
	state->info.stats->lives--;
}

// Διαχειρίζεται τη σύγκρουση αστεριού - διαστημοπλοίου
void handle_asteroid_star_collision(State state, int pos) {
	// Το αστέρι διαγράφεται
	vector_remove_at(state->objects, pos);
	// Το σκορ αυξάνεται κατά 1
	score_update(state, 1);
}

// Εντοπίζει τις συγκρούσεις 
void handle_collisions(State state) {

	state->info.events->collision = false;

	// Συντεταγμένες διαστημοπλοίου
    double sx = state_info(state)->spaceship->position.x;
    double sy = state_info(state)->spaceship->position.y;

	
	for (int i = 0; i < vector_size(state->objects); i++) {
		// Εντοπισμός συγκρούσεων αστεροειδή-σφαίρας
		if (((Object)vector_get_at(state->objects, i))->type == ASTEROID) {

			Object asteroid = vector_get_at(state->objects, i);
			bool destroyed  = false;
			
			for (int j = 0; j < vector_size(state->objects); j++) {
				if (((Object)vector_get_at(state->objects, j))->type != BULLET) continue;	

				Object bullet = vector_get_at(state->objects, j);

				if (collide(asteroid, bullet)) {
					handle_bullet_asteroid_collision(state, bullet, asteroid, i);
					// Αν ο αστεροειδής καταστραφεί, δεν μπορεί να συγκρουστεί ξανά με κάποιο αντικείμενο
					destroyed = true;
		
					// Αν η σύγκρουση ήταν ορατή, σημειώνουμε ότι συνέβη
					if (is_inside_rectangle(asteroid->position, (Vector2){sx-MID_WIDTH, sy+MID_HEIGHT}, (Vector2){sx+MID_WIDTH, sy-MID_HEIGHT})) {
						state->info.events->collision = true;
					}
					break;
				}

			}

			// Εντοπισμός συγκρούσεων αστεροειδή-διαστημοπλοίου
			if (!destroyed && collide(asteroid, state->info.spaceship)) {
				handle_asteroid_spaceship_collision(state, asteroid, i);

				// Αν η σύγκρουση ήταν ορατή, σημειώνουμε ότι συνέβη
				if (is_inside_rectangle(asteroid->position, (Vector2){sx-MID_WIDTH, sy+MID_HEIGHT}, (Vector2){sx+MID_WIDTH, sy-MID_HEIGHT})) {
					state->info.events->collision = true;
				}
			}	
		// Εντοπισμός συγκρούσεων διαστημοπλοίου-αστεριού
		} else if (((Object)vector_get_at(state->objects, i))->type == STAR) {
			Object star = (Object)vector_get_at(state->objects, i);
			if (collide(star, state->info.spaceship)) {
				handle_asteroid_star_collision(state, i);
				// Καταγράφουμε ότι ο παίκτης κέρδισε ένα αστέρι
				state->info.events->star = true;
			}
		}
	}
}

///// Συναρτήσεις διαγραφής αντικειμένων //////////////////////////////////////////////////////////////////////////////////////////////////////////

// Απελευθέρωση μνήμης που δεσμέυεται από ένα αντικείμενο
void destroy_object(Object obj) {
	free(obj->asteroid_state);
	free(obj);
}

// Διαγράφει τα αντικείμενα που βρίσκονται τουλάχιστον 2 οθόνες μακριά από το διαστημόπλοιο
void delete_objects(State state) {
	// Συντεταγμένες διαστημοπλοίου
	double sx = state->info.spaceship->position.x;
	double sy = state->info.spaceship->position.y;

	// Ορθογώνιο διαστάσεων 4 οθονών με κέντρο το διαστημόπλοιο
	Vector2 top_left = (Vector2){sx-2*SCREEN_WIDTH, sy+2*SCREEN_HEIGHT};
	Vector2 bottom_right = (Vector2){sx+2*SCREEN_WIDTH, sy-2*SCREEN_HEIGHT};

	for (int i = 0; i < vector_size(state->objects); i++) {
		Object object = vector_get_at(state->objects, i);
		// Αν το αντικείμενο δεν ανήκει στο ορθογώνιο, αφαιρείται
		if (!is_inside_rectangle(object->position, top_left, bottom_right)) {
				vector_remove_at(state->objects, i);
				i--;
		}
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη
void state_destroy(State state) {
	vector_destroy(state->objects);
	free(state->info.events);
	free(state->info.screen_state);
	free(state->info.stats);
	free(state->info.spaceship);
	free(state);
}

///// Κύρια συνάρτηση (state update) //////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.
void state_update(State state, KeyState keys, ButtonState buttons) {

	// Μεταβολή οθόνης ανάλογα με τα πατημένα κουμπιά
	if (buttons->play) {
		state->info.screen_state->screen = TRANSITION;
	} else if (buttons->rules) {
		state->info.screen_state ->screen = RULES;
	} else if (buttons->home) {
		state->info.screen_state->screen = WELCOME;
	} else if (buttons->credits) {
		state->info.screen_state->screen = CREDITS;
	}

	if (state->info.screen_state->screen == TRANSITION) {
		// Η παραμονή στην μεταβατική οθόνη διαρκεί 220 frames
		if (state->info.screen_state->frames_in_transition == 220) {
			state->info.screen_state->frames_in_transition = 0;
			// Ξεκινάει το παιχνίδι
			state->info.screen_state->screen = GAME;
		} else {
			// Αυξάνονται τα frames στη μεταβατική οθόνη
			state->info.screen_state->frames_in_transition++;
		}
	}

	// Το state αλλάζει κι άλλο, μόνο αν το παιχνίδι έχει αρχίσει
	if (state->info.screen_state->screen != GAME) return;

	// Αλλαγή κατάστασης του παιχνιδιού (paused - not paused)
	if (keys->p) {
		state->info.paused = !state->info.paused;
	}

	// Αν το n είναι πατημένο, αφηνούμε το τρέχον frame να εξελιχθεί
	if (keys->n) {
		state->info.paused = false;
	}

	if (state->info.paused) return;

	// Ελέγχουμε αν ο παίκτης έχασε
	if (game_over(state)) return;

	// Προσθήκη νέων αστεροειδών και αστεριών
	num_objects_update(state);
	
	// Συντεταγμένες αντικειμένων
	double sx = state->info.spaceship->position.x;
	double sy = state->info.spaceship->position.y;
	
	// Λίστα αντικειμένων στο ορθογώνιο με κέντρο το διαστημόπλοιο και πλευρές 4screen_width και 4screen_height
	List playing_objects = state_objects(state, (Vector2){sx - 2*GetScreenWidth(), sy + 2*GetScreenHeight()}, 
												(Vector2){sx + 2*GetScreenWidth(), sy - 2*GetScreenHeight()});
	int index = 0;
	for (ListNode node = list_first(playing_objects); node != LIST_EOF; node = list_next(playing_objects, node)) {
		Object current_object = list_node_value(playing_objects, node);
		object_update(state, current_object, index);
	}
	list_destroy(playing_objects);

	// Ενημέρωση κατάστασης διαστημοπλοίου
	spaceship_update(state->info.spaceship, keys, state);

	// Συγκρούσεις
	handle_collisions(state);

	state->info.events->bullet_thrown = false;
	// Προσθήκη νέας σφαίρας
	if (keys->space) {
		add_bullet(state);
	}

	// Ανανέωση αριθμού frames από την τελευταία σφαίρα που κέρδισε ο παίκτης
	frames_from_added_bullet_update(state);

	// Διαγράφουμε τα αντικείμενα που είναι μακριά από το διαστημόπλοιο
	delete_objects(state);
	
	// Μείωση του αριθμού frames που απαιτούνται για να επιτραπεί η επόμενη σφαίρα
	state->next_bullet--;

	// Αύξηση της ταχύτητας παιχνιδιού αν το σκορ φτάσει σε κάποιο πολλαπλάσιο του 100
	if (state->info.stats->score != 0 && state->info.stats->score % 100 == 0) {
		state->speed_factor *= 1.1;
	}

	// Αν το n είναι πατημένο, διακόπτουμε την εξέλιξη του παιχνιδιού μετά την εκτέλεση του τρέχοντος frame
	if (keys->n) {
		state->info.paused = true;
	}
}


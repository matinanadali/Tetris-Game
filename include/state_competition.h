#pragma once

#include "raylib.h"
#include "ADTList.h"

// Χαρακτηριστικά αντικειμένων
#define ASTEROID_NUM 6
#define ASTEROID_MIN_SIZE 10
#define ASTEROID_MAX_SIZE 80
#define ASTEROID_MIN_SPEED 1
#define ASTEROID_MAX_SPEED 1.5
#define ASTEROID_MIN_DIST 300
#define ASTEROID_MAX_DIST 400
#define BULLET_SPEED 10
#define BULLET_SIZE 3
#define BULLET_DELAY 15
#define SPACESHIP_SIZE 85
#define SPACESHIP_ROTATION (PI/32)
#define SPACESHIP_ACCELERATION 0.1
#define SPACESHIP_SLOWDOWN 0.98

#define ASTEROID_MAX_ROTATION_SPEED (PI/32)
#define ASTEROID_MIN_ROTATION_SPEED (-PI/32)
#define ASTEROID_WIDTH 90
#define ASTEROID_HEIGHT 90

#define SCREEN_WIDTH 900	// Πλάτος της οθόνης
#define SCREEN_HEIGHT 700	// Υψος της οθόνης
#define MID_WIDTH ( SCREEN_WIDTH / 2 )
#define MID_HEIGHT ( SCREEN_HEIGHT / 2 )

#define INITIAL_LIVES 5
#define INITIAL_BULLETS 3

#define STAR_NUM 2
#define STAR_SIZE 20

typedef enum {
	SPACESHIP, ASTEROID, BULLET, STAR
} ObjectType;

typedef enum {
	IDLE, JUMPING, FALLING, MOVING_UP, MOVING_DOWN
} VerticalMovement;

typedef enum {
	WELCOME, TRANSITION, GAME, RULES, GAME_OVER
} Screen;

typedef struct asteroid_state {
	int type;					// Τύπος αστεροειδούς (0 - 7)
	Vector2 rotation;			// Περιστροφή αστεροειδούς
	float rotation_speed;		// Γωνιακή ταχύτητα αστεροειδούς
} *AsteroidState;

typedef struct screen_state {
	Screen screen;
	int frames_in_transition;
} *ScreenState;

typedef struct stats {
	int bullets_left;
	int lives;
	int score;
} *Stats;

typedef struct events {
	bool collision;
	bool bullet_thrown;
	bool star;
	int bullet_added;
} *Events;


// Πληροφορίες για κάθε αντικείμενο
typedef struct object {
	ObjectType type;				// Τύπος (Διαστημόπλοιο, Αστεροειδής, Σφαίρα)
	AsteroidState asteroid_state;	// Πληροφορίες για τον αστεροειδή (αν το αντικείμενο είναι αστεροειδής)
	Vector2 position;				// Θέση
	Vector2 speed;					// Ταχύτητα (pixels/frame)
	double size;					// Μέγεθος (pixels)
	Vector2 orientation;			// Κατεύθυνση (μόνο για διαστημόπλοιο)
}* Object;

// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
typedef struct state_info {
	Object spaceship;				// πληροφορίες για τη το διαστημόπλοιο
	ScreenState screen_state;		// Πληροφορίες για το σε ποια οθόνη βρισκόμαστε (welcome, transition, game, rules)
	Events events;
	bool paused;					// true αν το παιχνίδι είναι paused
	Stats stats;				
}* StateInfo;

// Πληροφορίες για το ποια πλήκτρα είναι πατημένα
typedef struct key_state {
	bool up;						// true αν το αντίστοιχο πλήκτρο είναι πατημένο
	bool left;
	bool right;
	bool enter;
	bool space;
	bool n;
	bool p;
}* KeyState;

// Πληοροφορίες για το ποια κουμπιά της αρχικής οθόνης έχουν πατηθεί
typedef struct button_state {
	bool play;
	bool rules;
	bool home;
} *ButtonState;

// Η κατάσταση του παιχνιδιού (handle)
typedef struct state* State;


// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create();

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state);

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.

List state_objects(State state, Vector2 top_left, Vector2 bottom_right);

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys, ButtonState buttons);

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state);


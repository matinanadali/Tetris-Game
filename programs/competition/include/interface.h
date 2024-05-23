#include "state_competition.h"
#include "raylib.h"
#include "ADTList.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>

#define BUTTON_WIDTH 162
#define BUTTON_HEIGHT 70
#define SPACESHIP_WIDTH 99
#define SPACESHIP_HEIGHT 75
#define MID_WIDTH ( SCREEN_WIDTH / 2 )
#define MID_HEIGHT ( SCREEN_HEIGHT / 2 )
#define ASTEROID_WIDTH 90
#define ASTEROID_HEIGHT 90

extern Rectangle play_button_bounds;
extern Rectangle rules_button_bounds;
extern Rectangle home_button_bounds;
extern Texture spaceship;
extern Texture background;
extern Texture play_button;
extern Texture rules_button;
extern Texture home_button;
extern Texture asteroids;
extern Font font;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);
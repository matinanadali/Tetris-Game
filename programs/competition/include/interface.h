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

extern float scrolling_back;
extern Rectangle play_button_bounds;
extern Rectangle rules_button_bounds;
extern Rectangle home_button_bounds;
extern Texture spaceship;
extern Texture background;
extern Texture play_button;
extern Texture rules_button;
extern Texture home_button;
extern Texture asteroids;
extern Texture bullet_left;
extern Texture life;
extern Texture star;
extern Font font;
extern Sound collision_sound;
extern Sound bullet_sound;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);
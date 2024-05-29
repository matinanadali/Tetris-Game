#include "state_competition.h"
#include "raylib.h"
#include "ADTList.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>

#define BUTTON_WIDTH 162
#define BUTTON_HEIGHT 70
#define PLAY_AGAIN_BUTTON_WIDTH 297
#define SPACESHIP_WIDTH 99
#define SPACESHIP_HEIGHT 75
// Πάχος εικονιδίου διαθέσιμης σφαίρας
#define BULLET_LEFT_WIDTH 40
// Πάχος εικονιδίου ζωής παίκτη
#define LIFE_WIDTH 40

// Γραφικά που χρησιμοποιούνται σε διάφορες οθόνες - αρχεία
extern float scrolling_back;
extern Rectangle play_button_bounds;
extern Rectangle rules_button_bounds;
extern Rectangle home_button_bounds_in_rules;
extern Rectangle home_button_bounds_in_game_over;
extern Rectangle play_again_button_bounds;
extern Texture spaceship;
extern Texture bullet;
extern Texture background;
extern Texture play_button;
extern Texture rules_button;
extern Texture home_button;
extern Texture play_again_button;
extern Texture credits_button;
extern Texture asteroids;
extern Texture bullet_left;
extern Texture life;
extern Texture star;
// Γραμματοσειρά
extern Font font;
// 'Ηχοι
extern Sound collision_sound;
extern Sound bullet_sound;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);
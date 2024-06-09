#include "interface.h"
#include "welcome_screen.h"
#include "game_screen.h"
#include "rules_screen.h"
#include "transition_screen.h"
#include "game_over_screen.h"
#include "credits_screen.h"

float scrolling_back = 0.0f;

const Color bullet_color = (Color){165,58,57,232};

// Θέσεις κουμπιών
Rectangle play_button_bounds = {40, MID_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle rules_button_bounds = {100 + BUTTON_WIDTH, MID_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle credits_button_bounds = {40, MID_HEIGHT + BUTTON_HEIGHT + 30, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle credits_button_bounds_in_game_over = {MID_WIDTH - BUTTON_WIDTH * 0.5 - 35, 400, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle home_button_bounds_in_rules = {MID_WIDTH - 0.5 * BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT - 80, BUTTON_WIDTH,  BUTTON_HEIGHT};
Rectangle home_button_bounds_in_game_over = {MID_WIDTH - 0.5 * BUTTON_WIDTH, 300, BUTTON_WIDTH,  BUTTON_HEIGHT};
Rectangle play_again_button_bounds = {MID_WIDTH - 0.5 * PLAY_AGAIN_BUTTON_WIDTH, 200, PLAY_AGAIN_BUTTON_WIDTH,  BUTTON_HEIGHT};
// Γραφικά που χρησιμοποιόυνται
Texture spaceship;
Texture asteroids;
Texture background;
Texture rules_button;
Texture play_button;
Texture home_button;
Texture play_again_button;
Texture credits_button;
Texture bullet_left;
Texture life;
Texture star;
Texture bullet;
// Γραμματοσειρά
Font font;
// Ήχοι
Sound collision_sound;
Sound bullet_sound;
// Μουσική
Music music;

void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

    // Φόρτωση γραφικών
    spaceship = LoadTexture("assets/spaceship.png");
    
    background = LoadTexture("assets/background.png");
    rules_button = LoadTexture("assets/rules_button.png");
    play_button = LoadTexture("assets/play_button.png");
    home_button = LoadTexture("assets/home_button.png");
    play_again_button = LoadTexture("assets/play_again_button.png");
    credits_button = LoadTexture("assets/credits_button.png");
    asteroids = LoadTexture("assets/asteroids.png");
    bullet = LoadTexture("assets/bullet.png");
    bullet_left = LoadTexture("assets/bullet_left.png");
    life = LoadTexture("assets/life.png");
    star = LoadTexture("assets/star.png");

    // Φόρτωση ήχων
    collision_sound = LoadSound("assets/collision_sound.wav");
    bullet_sound = LoadSound("assets/bullet_sound.wav");

    // Φόρτωση γραμματοσειράς
    font = LoadFont("assets/ARCADE_N.TTF");
   
    InitAudioDevice();    
    // Φόρτωση μουσικής
    music = LoadMusicStream("assets/cover.mp3");
    PlayMusicStream(music);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}


void interface_draw_frame(State state) {
    UpdateMusicStream(music);
    BeginDrawing();

    // Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(BLACK);

    // Κλήση αντίστοιχης συνάρτησης ανάλογα με την οθόνη στην οποία βρισκόμαστε
    if (state_info(state)->screen_state->screen == WELCOME) {
        draw_welcome_screen(state);
    } else if (state_info(state)->screen_state->screen == GAME) {
        draw_game_screen(state);
    } else if (state_info(state)->screen_state->screen == TRANSITION) {
        draw_transition_screen(state);
    } else if (state_info(state)->screen_state->screen == RULES) {
        draw_rules_screen(state);
    } else if (state_info(state)->screen_state->screen == CREDITS) {
        draw_credits_screen(state);
    } else {
        draw_game_over_screen(state);
    }
	
    EndDrawing();
}


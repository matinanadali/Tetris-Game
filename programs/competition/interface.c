#include "interface.h"
#include "welcome_screen.h"
#include "game_screen.h"
#include "rules_screen.h"
#include "transition_screen.h"
#include "game_over_screen.h"

Music music;

Rectangle play_button_bounds = {40, MID_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle rules_button_bounds = {80 + BUTTON_WIDTH, MID_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
Rectangle home_button_bounds_in_rules = {MID_WIDTH - (BUTTON_WIDTH / 2), SCREEN_HEIGHT - BUTTON_HEIGHT - 80, BUTTON_WIDTH,  BUTTON_HEIGHT};
Rectangle home_button_bounds_in_game_over = {MID_WIDTH - (BUTTON_WIDTH / 2), 300, BUTTON_WIDTH,  BUTTON_HEIGHT};
Rectangle play_again_button_bounds = {MID_WIDTH - PLAY_AGAIN_BUTTON_WIDTH/2, 200, PLAY_AGAIN_BUTTON_WIDTH,  BUTTON_HEIGHT};
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
Font font;
Sound collision_sound;
Sound bullet_sound;

void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    spaceship = LoadTexture("assets/spaceship.png");
    // pexels.com Photo by Kai Pilger: https://www.pexels.com/photo/cluster-of-stars-1341279/
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
    font = LoadFont("assets/ARCADE_N.TTF");
    star = LoadTexture("assets/star.png");
    collision_sound = LoadSound("assets/collision_sound.wav");
    bullet_sound = LoadSound("assets/bullet_sound.wav");
   
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

    if (state_info(state)->screen_state->screen == WELCOME) {
        draw_welcome_screen(state);
    } else if (state_info(state)->screen_state->screen == GAME) {
        draw_game_screen(state);
    } else if (state_info(state)->screen_state->screen == TRANSITION) {
        draw_transition_screen(state);
    } else if (state_info(state)->screen_state->screen == RULES) {
        draw_rules_screen(state);
    } else {
        draw_game_over_screen(state);
    }
	
    EndDrawing();
}


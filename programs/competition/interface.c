#include "raylib.h"
#include "ADTList.h"
#include "state_competition.h"
#include "welcome_screen.h"
#include "game_screen.h"
#include "interface.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>

Music music;


Rectangle play_button_bounds = {40, SCREEN_HEIGHT / 2, BUTTON_WIDTH + 80, BUTTON_HEIGHT + SCREEN_HEIGHT/ 2};
Rectangle rules_button_bounds = {80 + BUTTON_WIDTH, SCREEN_HEIGHT / 2, 2*BUTTON_WIDTH, BUTTON_HEIGHT + SCREEN_HEIGHT / 2};
Texture spaceship;
Texture background;
Texture rules_button;
Texture play_button;


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    spaceship = LoadTexture("assets/spaceship.png");
    // pexels.com Photo by Kai Pilger: https://www.pexels.com/photo/cluster-of-stars-1341279/
    background = LoadTexture("assets/background.png");
    rules_button = LoadTexture("assets/rules_button.png");
    play_button = LoadTexture("assets/play_button.png");
   

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

    if (state_info(state)->welcome) {
        draw_welcome_screen(state);
    } else {
        draw_game_screen(state);
    }
	
    EndDrawing();
}


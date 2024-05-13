#include "raylib.h"
#include "ADTList.h"
#include "state_comp.h"
#include "interface.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define SPACESHIP_WIDTH 100
#define SPACESHIP_HEIGHT 37

#define MOVING_WIDTH 79
#define MOVING_HEIGHT 10

#define ASTEROID_WIDTH 90
#define ASTEROID_HEIGHT 90

Music music;
Texture spaceship;
Texture moving_effect;
Texture asteroids;

static bool double_equal(double a, double b) {
	return fabs(a-b) < 1e-6;
}

// Ελέγχει την ισότητα δύο διανυσμάτων
static bool vec2_equal(Vector2 a, Vector2 b) {
	return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);

    InitAudioDevice();    
    // Φόρτωση μουσικής
    music = LoadMusicStream("programs/assets/cover.mp3");
    PlayMusicStream(music);
    spaceship = LoadTexture("programs/assets/spaceship.png");
    moving_effect = LoadTexture("programs/assets/moving_effect.png");
    asteroids = LoadTexture("programs/assets/asteroids.png");
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

float radians_to_degrees(double angle_in_radians) {
    return 180 * angle_in_radians / PI;
}

void interface_draw_frame(State state) {
    UpdateMusicStream(music);
    BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(BLACK);
    int width = GetScreenWidth();
    int height = GetScreenHeight();

	// Σχεδιάζουμε το διαστημόπλοιο

    // Η γωνία που σχηματίζει το διάνυσμα orientation με τον θετικό ημιάξονα x
    double angle = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x);

    DrawTexturePro(spaceship, (Rectangle){0, 0, SPACESHIP_WIDTH, SPACESHIP_HEIGHT}, 
                              (Rectangle){width/2, height/2, SPACESHIP_WIDTH, SPACESHIP_HEIGHT}, 
                              (Vector2){SPACESHIP_WIDTH/2, SPACESHIP_HEIGHT/2},
                              -radians_to_degrees(angle), WHITE);
    Vector2 speed = state_info(state)->spaceship->speed;
    Vector2 top_left_corner = (Vector2){-SPACESHIP_WIDTH/2, -SPACESHIP_HEIGHT/2 + 7};
    top_left_corner = vec2_rotate(top_left_corner, -angle);
    top_left_corner = vec2_add(top_left_corner, (Vector2){width/2, height/2});

    Vector2 bottom_left_corner = (Vector2){-SPACESHIP_WIDTH/2, SPACESHIP_HEIGHT/2 - 7};
    bottom_left_corner = vec2_rotate(bottom_left_corner, -angle);
    bottom_left_corner = vec2_add(bottom_left_corner, (Vector2){width/2, height/2});

    if (!vec2_equal(speed, (Vector2){0,0})) {
        DrawTexturePro(moving_effect, (Rectangle){0, 0, MOVING_WIDTH, MOVING_HEIGHT},
                                      (Rectangle){top_left_corner.x, top_left_corner.y, MOVING_WIDTH, MOVING_HEIGHT},
                                      (Vector2){MOVING_WIDTH, 0},
                                      -radians_to_degrees(angle), WHITE);
        DrawTexturePro(moving_effect, (Rectangle){0, 0, MOVING_WIDTH, MOVING_HEIGHT},
                                      (Rectangle){bottom_left_corner.x, bottom_left_corner.y, MOVING_WIDTH, MOVING_HEIGHT},
                                      (Vector2){MOVING_WIDTH, MOVING_HEIGHT},
                                      -radians_to_degrees(angle), WHITE);
    }

    // Συντεταγμένες διαστημοπλοίου
    double sx = state_info(state)->spaceship->position.x;
    double sy = state_info(state)->spaceship->position.y;
   
    // Σχεδιάζουμε τα υπόλοιπα αντικείμενα (σφαίρες και αστεροειδείς)

    // Ορθογώνιο μέσα στο οποίο τα αντικείμενα είναι ορατά
    Vector2 top_left = (Vector2){sx-width/2, sy+height/2};
    Vector2 bottom_right = (Vector2){sx+width/2, sy-height/2};

    // Λίστα ορατών αντικειμένων
    List objects = state_objects(state, top_left, bottom_right);

    for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
        Object object = list_node_value(objects, node);
        // Εφόσον μεταφέραμε την αρχή των αξόνων στο (width/2 - sx, height/2 + sy), οι συντεταγμένες κάθε αντικειμένου 
        // μεταβάλλονται ανάλογα
        // Επειδή η βιβλιοθήκη θεωρεί ότι τα y αυξάνονται προς τα κάτω, 
        // αλλάζουμε πρόσημο στις y συντεταγμένες των αντικειμένων
        if (object->type == BULLET) {
            DrawCircle(object->position.x-sx+width/2, -object->position.y +sy+height/2, object->size, WHITE);
        } else {
            DrawTexturePro(asteroids,(Rectangle){(object->asteroid_type)*ASTEROID_WIDTH, 0, ASTEROID_WIDTH, ASTEROID_HEIGHT},
                                    (Rectangle){object->position.x-sx+width/2, -object->position.y +sy+height/2, object->size, object->size * 1.5},
                                    (Vector2){0,0}, 
                                    0, WHITE);
        }
       
    }

    // Αποδέσμευση μνήμης
    list_destroy(objects);
    // Σχεδιάζουμε το σκορ
    DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);
	
    EndDrawing();
}


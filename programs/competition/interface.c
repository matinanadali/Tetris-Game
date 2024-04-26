#include "raylib.h"
#include "ADTList.h"
#include "state.h"
#include "interface.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>


#define IMG_WIDTH 100
#define IMG_HEIGHT 37

Music music;
Texture spaceship;
Texture moving_effect;

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
    music = LoadMusicStream("../game/assets/cover.mp3");
    PlayMusicStream(music);

    spaceship = LoadTexture("assets/spaceship.png");
    moving_effect = LoadTexture("assets/moving_effect.png");
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

// Μεταβιβάζει τα ορίσματα pointA, pointB και pointC στη συνάρτηση βιβλιοθήκης DrawTrianlgeLines
// με τη σωστή σειρά (counter-clockwise)
void DrawTriangleLinesHelper(Vector2 pointA, Vector2 pointB, Vector2 pointC) {
    double slope1 = (pointB.y - pointA.y) * (pointC.x - pointB.x);
    double slope2 = (pointC.y - pointB.y) * (pointB.x - pointA.x);
    if (slope1 > slope2) {
        DrawTriangleLines(pointA, pointB, pointC, WHITE);
    }
    else {
        DrawTriangleLines(pointA, pointC, pointB, WHITE);
    }
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
  
    DrawTexturePro(spaceship, (Rectangle){0, 0, IMG_WIDTH, IMG_HEIGHT}, 
                              (Rectangle){width/2, height/2, IMG_WIDTH, IMG_HEIGHT}, 
                              (Vector2){IMG_WIDTH/2, 0},
                              -radians_to_degrees(angle), WHITE);

    Vector2 speed = state_info(state)->spaceship->speed;
    if (!vec2_equal(speed, (Vector2){0,0})) {
        DrawTexturePro(moving_effect, (Rectangle){0, 0, 126, 16},
                                      (Rectangle){width/2-126, height/2+IMG_HEIGHT/3, 126, 16},
                                      (Vector2){126, 16},
                                      -radians_to_degrees(angle), WHITE);
        DrawTexturePro(moving_effect, (Rectangle){0, 0, 126, 16},
                                      (Rectangle){width/2-126, height/2-IMG_HEIGHT/3, 126, 16},
                                      (Vector2){126, 16},
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
            DrawCircleLines(object->position.x-sx+width/2, -object->position.y +sy+height/2, object->size, WHITE);
        }
       
    }

    // Αποδέσμευση μνήμης
    list_destroy(objects);
    // Σχεδιάζουμε το σκορ
    DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);
	
    EndDrawing();
}


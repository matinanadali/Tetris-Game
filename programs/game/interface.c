#include "raylib.h"
#include "ADTList.h"
#include "state.h"
#include "interface.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>

// Texture spaceship_img;
void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
//	SetTargetFPS(60);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

bool is_zero(double a) {
	return fabs(a) < 1e-6;
}
void interface_draw_frame(State state) {
    BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(RAYWHITE);

	// Σχεδιάζουμε το διαστημόπλοιο
    Vector2 v = (Vector2){SCREEN_WIDTH / 2 - state_info(state)->spaceship->position.x, 
                            SCREEN_HEIGHT / 2 - state_info(state)->spaceship->position.y};
    // double sx = state_info(state)->spaceship->orientation.x;
    // double sy = state_info(state)->spaceship->orientation.y;
    // double angle;
    // if (is_zero(sx)) {
    //     if (sy > 0) {
    //         angle = PI / 2;
    //     } else {
    //         angle = -PI / 2;
    //     }
    // } else {
    //     angle = atan2(sy, sx);
    // }

    // Vector2 point = vec2_from_polar(30, angle + PI / 2);

    // Vector2 pointC = vec2_from_polar(10, angle + PI);
    // Vector2 pointB = vec2_from_polar(10, angle);

   // DrawTriangle(vec2_add(point, v), vec2_add(pointB, v), vec2_add(pointC, v), WHITE);
    // Σχεδιάζουμε τα υπόλοιπα αντικείμενα (σφαίρες και αστεροειδείς)
    Vector2 top_left = vec2_add(v, (Vector2){-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    Vector2 bottom_right = vec2_add(v, (Vector2){SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2});
    List objects = state_objects(state, top_left, bottom_right);

    for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
        Object object = list_node_value(objects, node);
        Color color;
        if (object->type == BULLET) {
            color = RED;
        } else {
            color = GREEN;
        }
        DrawCircle(object->position.x + v.x, object->position.y + v.y, object->size, color);
    }
list_destroy(objects);
    //Σχεδιάζουμε το σκορ
    DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);
	
   EndDrawing();
}


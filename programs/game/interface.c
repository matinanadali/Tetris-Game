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
	SetTargetFPS(60);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

bool is_zero(double a) {
	return fabs(a) < 1e-6;
}

void draw_triangle(Vector2 pointA, Vector2 pointB, Vector2 pointC) {
    int val = (pointA.x - pointB.x) * (pointA.y + pointB.y)
              + (pointC.x - pointA.x) * (pointC.y + pointA.y);
 
    if (val > 0) {
        DrawTriangle(pointA, pointB, pointC, WHITE);
    } else {
        DrawTriangle(pointA, pointC, pointB, WHITE);
    }
 
}
void interface_draw_frame(State state) {
    BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(BLACK);
    int width = GetScreenWidth();
    int height = GetScreenHeight();

	// Σχεδιάζουμε το διαστημόπλοιο
    Vector2 v = (Vector2){width / 2 - state_info(state)->spaceship->position.x, 
                            height / 2 - state_info(state)->spaceship->position.y};
    double angle = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x);

    Vector2 point = {0, 30};
    if (angle <= -PI / 2) {
        angle = 2*PI + angle;
    }
    point = vec2_rotate(point, angle - PI / 2);
    point = vec2_scale(point, -1);
    point = vec2_add(point, v);
    
    
    Vector2 pointB = {10, 0};
    Vector2 pointC = {-10, 0};
   
    pointB = vec2_rotate(pointB, angle - PI / 2);
    pointB = vec2_add(pointB, v);
    
    
    pointC = vec2_rotate(pointC, angle - PI / 2);
    pointC = vec2_add(pointC, v);
    
    draw_triangle(point, pointB, pointC);
   
   
   
   
    // Σχεδιάζουμε τα υπόλοιπα αντικείμενα (σφαίρες και αστεροειδείς)
    Vector2 top_left = vec2_add(v, (Vector2){- width/ 2, height / 2});
    Vector2 bottom_right = vec2_add(v, (Vector2){width / 2, -height / 2});
    List objects = state_objects(state, top_left, bottom_right);

    for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
        // Object object = list_node_value(objects, node);
        // Color color;
        // if (object->type == BULLET) {
        //     color = RED;
        // } else {
        //     color = GREEN;
        // }
     //   DrawCircle(object->position.x + v.x, object->position.y + v.y, object->size, color);
    }
    list_destroy(objects);
    //Σχεδιάζουμε το σκορ
    DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);
	
   EndDrawing();
}


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
   double slope1 = (pointB.y - pointA.y) * (pointC.x - pointB.x);
double slope2 = (pointC.y - pointB.y) * (pointB.x - pointA.x);

    if (slope1 > slope2) {
        DrawTriangleLines(pointA, pointB, pointC, WHITE);
    }
    else {
        DrawTriangleLines(pointA, pointC, pointB, WHITE);
    }
 
}
void interface_draw_frame(State state) {
    BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(BLACK);
    int width = GetScreenWidth();
    int height = GetScreenHeight();

	// Σχεδιάζουμε το διαστημόπλοιο
  //  Vector2 new_origin = (Vector2){+width / 2-state_info(state)->spaceship->position.x, +height / 2-state_info(state)->spaceship->position.y};
    double angle = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x);

    Vector2 point = {0, -40};
    Vector2 pointB = {20, 0};
    Vector2 pointC = {-20,0};
    
     

    point = vec2_rotate(point, -angle + PI / 2);
    pointB = vec2_rotate(pointB, -angle + PI / 2);
    pointC = vec2_rotate(pointC, -angle + PI / 2);

    point = vec2_add(point, (Vector2){width/2, height/2}); 
    pointB = vec2_add(pointB, (Vector2){width/2, height/2});
    pointC = vec2_add(pointC, (Vector2){width/2, height/2});
  
    
    draw_triangle(pointB, pointC, point);
   double sx = state_info(state)->spaceship->position.x;
    double sy = state_info(state)->spaceship->position.y;
    // Σχεδιάζουμε τα υπόλοιπα αντικείμενα (σφαίρες και αστεροειδείς)
    Vector2 top_left = (Vector2){-width/2+sx, sy+height/2};
    Vector2 bottom_right = (Vector2){sx+width/2, -height/2+sy};
    List objects = state_objects(state, top_left, bottom_right);

    for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
        Object object = list_node_value(objects, node);
        if (object->type == BULLET) {
            DrawCircle(object->position.x-sx+width/2, -object->position.y +sy+height/2, object->size, WHITE);
        } else {
            DrawCircleLines(object->position.x-sx+width/2, -object->position.y +sy+height/2, object->size, WHITE);
        }
       
    }
    list_destroy(objects);
    //Σχεδιάζουμε το σκορ
    DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);
	
   EndDrawing();
}


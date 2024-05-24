#include "game_screen.h"
#define BULLET_LEFT_WIDTH 40
#define LIFE_WIDTH 40
const Color bullet_color = (Color){165,58,57,232};
void draw_background(State state) {
    int sx = state_info(state)->spaceship->position.x;
    int sy = state_info(state)->spaceship->position.y;
    sx = sx % SCREEN_WIDTH;
    sy = sy % SCREEN_HEIGHT;
    int dx[9] = {0,0,0,1,1,1,-1,-1,-1};
    int dy[9] = {0,-1,1,0, -1, 1, 0, -1, 1};
    for (int i = 0; i < 9; i++) {
        DrawTextureEx(background, (Vector2){-sx + dx[i] * SCREEN_WIDTH, sy + dy[i] * SCREEN_HEIGHT}, 0.0f, 2.0f, (Color){255,255,255, 70});
    }
}

void draw_spaceship(State state) {
    // Η γωνία που σχηματίζει το διάνυσμα orientation με τον θετικό ημιάξονα x
    double angle = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x);
    // Προσαρμογή της γωνίας στο σύστημα συντεταγμένων της raylib
    angle = 90 - angle * RAD2DEG;

    Rectangle spaceship_source = (Rectangle){0,0,SPACESHIP_WIDTH, SPACESHIP_HEIGHT};
    Rectangle spaceship_dest = (Rectangle){MID_WIDTH,MID_HEIGHT, SPACESHIP_WIDTH, SPACESHIP_HEIGHT};
    Vector2 spaceship_origin = (Vector2){SPACESHIP_WIDTH/2, SPACESHIP_HEIGHT/2};
    DrawTexturePro(spaceship, spaceship_source, spaceship_dest, spaceship_origin, angle, WHITE);
}

void draw_asteroid(double sx, double sy, Object asteroid) {
    int type = asteroid->asteroid_state->type;
    double angle = atan2(asteroid->asteroid_state->rotation.y, asteroid->asteroid_state->rotation.x);
    // Προσαρμογή της γωνίας στο σύστημα συντεταγμένων της raylib
    angle = 90 - angle * RAD2DEG;

    float scale_ratio = asteroid->size / ASTEROID_MAX_SIZE;
    float width = ASTEROID_WIDTH * scale_ratio;
    float height = ASTEROID_HEIGHT * scale_ratio;

    if (asteroid->asteroid_state->frames_destroyed == 0) {
        Rectangle asteroid_source = (Rectangle){type * ASTEROID_WIDTH, 0, ASTEROID_WIDTH, ASTEROID_HEIGHT};
        Rectangle asteroid_dest = (Rectangle){asteroid->position.x-sx+MID_WIDTH, 
                                              -asteroid->position.y+sy+MID_HEIGHT, 
                                              width, 
                                              height};
        Vector2 asteroid_origin = (Vector2){width * 0.5, height * 0.5};
        DrawTexturePro(asteroids, asteroid_source, asteroid_dest, asteroid_origin, angle, WHITE);
    } 
}

void draw_bullet(double sx, double sy, Object bullet_obj) {
    double angle = atan2(bullet_obj->orientation.y, bullet_obj->orientation.x);
    // Προσαρμογή της γωνίας στο σύστημα συντεταγμένων της raylib
    angle = 90 - angle * RAD2DEG;

    Rectangle bullet_source = (Rectangle){0, 0, BULLET_WIDTH, BULLET_HEIGHT};
    Rectangle bullet_dest = (Rectangle){bullet_obj->position.x-sx+MID_WIDTH, 
                                        -bullet_obj->position.y+sy+MID_HEIGHT, 
                                        BULLET_WIDTH, 
                                        BULLET_HEIGHT};
    Vector2 bullet_origin = (Vector2){BULLET_WIDTH/2, BULLET_HEIGHT/2};   
    DrawTexturePro(bullet, bullet_source, bullet_dest, bullet_origin, angle, WHITE);                          
}

void draw_objects(State state) {
    // Συντεταγμένες διαστημοπλοίου
    double sx = state_info(state)->spaceship->position.x;
    double sy = state_info(state)->spaceship->position.y;
   
    // Σχεδιάζουμε τα υπόλοιπα αντικείμενα (σφαίρες και αστεροειδείς)

    // Ορθογώνιο μέσα στο οποίο τα αντικείμενα είναι ορατά
    Vector2 top_left = (Vector2){sx-MID_WIDTH, sy+MID_HEIGHT};
    Vector2 bottom_right = (Vector2){sx+MID_WIDTH, sy-MID_HEIGHT};

    // Λίστα ορατών αντικειμένων
    List objects = state_objects(state, top_left, bottom_right);

    for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
        Object object = list_node_value(objects, node);
        // Εφόσον μεταφέραμε την αρχή των αξόνων στο (MID_WIDTH - sx, MID_HEIGHT + sy), οι συντεταγμένες κάθε αντικειμένου 
        // μεταβάλλονται ανάλογα
        // Επειδή η βιβλιοθήκη θεωρεί ότι τα y αυξάνονται προς τα κάτω, 
        // αλλάζουμε πρόσημο στις y συντεταγμένες των αντικειμένων
        if (object->type == BULLET) {
            draw_bullet(sx, sy, object);
        } else if (object->type == ASTEROID) {
            draw_asteroid(sx, sy, object);
        } else {
            DrawTexture(star, object->position.x-sx+MID_WIDTH, -object->position.y +sy+MID_HEIGHT, WHITE);
        }
       
    }

    // Αποδέσμευση μνήμης
    list_destroy(objects);
}

void play_sounds(State state) {
    if (state_info(state)->events->bullet_thrown) {
        PlaySound(bullet_sound);
    }
    if (state_info(state)->events->collision) {
        PlaySound(collision_sound);
    } 
}

void draw_game_stats(State state) {
    int i;
    for (i = 0; i < state_info(state)->stats->bullets_left; i++) {
        DrawTexture(bullet_left, i * BULLET_LEFT_WIDTH + 40, 40, WHITE);
    }

    if (state_info(state)->events->bullet_added != 0) {
        DrawTextEx(font, "+1!", (Vector2){i * BULLET_LEFT_WIDTH + 40, 30}, 40, 0, WHITE);
    }

    for (i = 0; i < state_info(state)->stats->lives; i++) {
        DrawTexture(life, SCREEN_WIDTH - i * LIFE_WIDTH - 40, 40, WHITE);
    }
    DrawText(TextFormat("%d", state_info(state)->stats->score), MID_WIDTH, 40, 40, WHITE); 
}

void draw_game_screen(State state) {
    draw_background(state);
    draw_objects(state);
    draw_spaceship(state);
    play_sounds(state);
    draw_game_stats(state);
}
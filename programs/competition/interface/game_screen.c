#include "game_screen.h"

// Σχεδίαζει το background
void draw_background(State state) {
    // Συντεταγμένες διατησμοπλοίου - αρχή συστήματος συντεταγμένων
    int sx = state_info(state)->spaceship->position.x;
    int sy = state_info(state)->spaceship->position.y;
    sx = sx % SCREEN_WIDTH;
    sy = sy % SCREEN_HEIGHT;

    // Επιπλέον θέσεις όπου θα σχεδιαστεί η εικόνα του background ώστε να καλυφθεί όλη η οθόνη
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

    // Σχεδιάζουμε ολόκληρο το διαστημόπλοιο
    Rectangle spaceship_source = (Rectangle){0,0,SPACESHIP_WIDTH, SPACESHIP_HEIGHT};
    // Θέση διαστημοπλοίου
    Rectangle spaceship_dest = (Rectangle){MID_WIDTH,MID_HEIGHT, SPACESHIP_WIDTH, SPACESHIP_HEIGHT};
    // Σημείο περιστροφής είναι το κέντρο του διαστημοπλοίου
    Vector2 spaceship_origin = (Vector2){SPACESHIP_WIDTH * 0.5, SPACESHIP_HEIGHT * 0.5};
    // Σχεδίαση διαστημοπλοίου
    DrawTexturePro(spaceship, spaceship_source, spaceship_dest, spaceship_origin, angle, WHITE);
}

void draw_asteroid(double sx, double sy, Object asteroid) {
    int type = asteroid->asteroid_state->type;
    // Γωνία περσιτροφής
    double angle = atan2(asteroid->asteroid_state->rotation.y, asteroid->asteroid_state->rotation.x);
    // Προσαρμογή της γωνίας στο σύστημα συντεταγμένων της raylib
    angle = 90 - angle * RAD2DEG;

    // Προσαρμογή μεγέθους
    float scale_ratio = asteroid->size / ASTEROID_MAX_SIZE;
    float width = ASTEROID_WIDTH * scale_ratio;
    float height = ASTEROID_HEIGHT * scale_ratio;

    // Σχεδιάζουμε το μέρος της εικόνας των αστεροιειδών που εικονίζει τον αστεροειδή του συγκεκριμένου τύπου
    Rectangle asteroid_source = (Rectangle){type * ASTEROID_WIDTH, 0, ASTEROID_WIDTH, ASTEROID_HEIGHT};
    // Θέση αστεροειδούς
    Rectangle asteroid_dest = (Rectangle){asteroid->position.x-sx+MID_WIDTH, 
                                          -asteroid->position.y+sy+MID_HEIGHT, 
                                           width, 
                                          height};
    // Σημείο περιστροφής είναι το κέντρο του αστεροειδούς
    Vector2 asteroid_origin = (Vector2){width * 0.5, height * 0.5};
    // Σχεδίαση αστεροειδούς
    DrawTexturePro(asteroids, asteroid_source, asteroid_dest, asteroid_origin, angle, WHITE);

}

void draw_bullet(double sx, double sy, Object bullet_obj) {
    double angle = atan2(bullet_obj->orientation.y, bullet_obj->orientation.x);
    // Προσαρμογή της γωνίας στο σύστημα συντεταγμένων της raylib
    angle = 90 - angle * RAD2DEG;

    // Σχεδιάζουμε όλη την εικόνα της σφαίρας
    Rectangle bullet_source = (Rectangle){0, 0, BULLET_WIDTH, BULLET_HEIGHT};
    // Θέση σφαίρας
    Rectangle bullet_dest = (Rectangle){bullet_obj->position.x-sx+MID_WIDTH, 
                                        -bullet_obj->position.y+sy+MID_HEIGHT, 
                                        BULLET_WIDTH, 
                                        BULLET_HEIGHT};
    // Σημείο περιστροφής είναι το κέντρο της σφαίρας
    Vector2 bullet_origin = (Vector2){BULLET_WIDTH * 0.5, BULLET_HEIGHT * 0.5};  
    // Σχεδίαση σφαίρας 
    DrawTexturePro(bullet, bullet_source, bullet_dest, bullet_origin, angle, WHITE);                          
}

void draw_star(double sx, double sy, Object star_obj) {
    // Θέση αστεριού
    Vector2 star_dest = (Vector2){star_obj->position.x-sx+MID_WIDTH, 
                                  -star_obj->position.y+sy+MID_HEIGHT};
    // Σχεδίαση αστεριού
    DrawTexture(star, star_dest.x, star_dest.y, WHITE); 
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
            draw_star(sx, sy, object);
        }
       
    }

    // Αποδέσμευση μνήμης
    list_destroy(objects);
}

// Παίζει ήχους ανάλογα με τα events που συνέβησαν στο συγκεκριμένο state
void play_sounds(State state) {
    if (state_info(state)->events->bullet_thrown) {
        PlaySound(bullet_sound);
    }
    if (state_info(state)->events->collision) {
        PlaySound(collision_sound);
    } 
}

// Σχεδιάζει το σκορ, τις ζωές και τις διαθέσιμες σφαίρες
void draw_game_stats(State state) {
    int i;
    // Σχεδίαση διαθέσιμων σφαιρών
    for (i = 0; i < state_info(state)->stats->bullets_left; i++) {
        DrawTexture(bullet_left, i * BULLET_LEFT_WIDTH + 40, 40, WHITE);
    }

    // Αν έχει πρόσφατα ( < 1s πριν) προστεθεί σφαίρα, σχεδιάζουμε +1
    if (state_info(state)->events->bullet_added != 0) {
        DrawTextEx(font, "+1!", (Vector2){i * BULLET_LEFT_WIDTH + 40, 30}, 40, 0, WHITE);
    }

    // Σχεδίαση ζωών
    for (i = 0; i < state_info(state)->stats->lives; i++) {
        DrawTexture(life, SCREEN_WIDTH - i * LIFE_WIDTH - 40, 40, WHITE);
    }

    // Σχεδίαση σκορ
    DrawText(TextFormat("%d", state_info(state)->stats->score), MID_WIDTH, 40, 40, WHITE); 
}

// Κεντρική συνάρτηση : σχεδίαση στοιχεών state
void draw_game_screen(State state) {
    draw_background(state);
    draw_objects(state);
    draw_spaceship(state);
    play_sounds(state);
    draw_game_stats(state);
}
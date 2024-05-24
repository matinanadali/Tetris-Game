#include "transition_screen.h"
#define TOTAL_MOVING_SECONDS 40.0

// Σχεδιάζει τόξο κύκλου - παράμετρος segments καθορίζει πόσο λείος θα είναι
static void DrawArc(int centerX, int centerY, float radius, float startAngle, float endAngle, int segments, Color color) {
    float angleStep = (endAngle - startAngle) / segments;

    for (int i = 0; i < segments; i++) {
        float angle1 = startAngle + angleStep * i;
        float angle2 = startAngle + angleStep * (i + 1);

        float x1 = centerX + cosf(angle1) * radius;
        float y1 = centerY + sinf(angle1) * radius;
        float x2 = centerX + cosf(angle2) * radius;
        float y2 = centerY + sinf(angle2) * radius;

        DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 10, color);
    }
}

// Σχεδιάζει το αριστερό μέρος της οθόνης
void draw_counter_section(State state) {
    // Σχεδιάζουμε το background
    scrolling_back += (state_info(state)->screen_state->frames_in_transition > 180) ? 5.0f : 0.2f;
    if (scrolling_back >= 2*SCREEN_HEIGHT) scrolling_back = 0;
    DrawTextureEx(background, (Vector2){ 0, scrolling_back }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scrolling_back}, 0.0f, 2.0f, (Color){255,255,255, 70});

    // Σε ποιο δευτερόλεπτο βρίσκεται η μέτρηση
    int second = 3 - state_info(state)->screen_state->frames_in_transition / 60;
    float end_angle;

    // Τι θα γράφει το χρονόμετρο
    char string_second[10];
    
    if (second == 0) {
        snprintf(string_second, sizeof(string_second), "GO!");
        end_angle = 0;  // Ώστε να μη σχεδιαστεί κύκλος
    } else {
        snprintf(string_second, sizeof(string_second), "%d", second);
        end_angle = 2 * PI * ((state_info(state)->screen_state->frames_in_transition % 60) / 60.0f);
    }

    // Μέγεθος κειμένου
    Vector2 size = MeasureTextEx(font, string_second, 100, 0);
    // Θέση κειμένου
    Vector2 text_position = (Vector2){ MID_WIDTH / 2 - size.x / 2 + 40, MID_HEIGHT - size.y / 2 };
    // Κέντρο τόξου
    Vector2 center = vec2_add(text_position, vec2_scale(size, 0.5f));

    // Σχεδίαση τόξου
    DrawArc(center.x, center.y, 100, -PI / 2, -PI / 2 + end_angle, 72, WHITE);
    // Σχεδίαση κειμένου
    DrawTextEx(font, string_second, text_position, 100, 0, WHITE); 
}

void draw_moving_spaceship(State state) {
    int seconds_moving = state_info(state)->screen_state->frames_in_transition - 180;
    // Ταχύτητα διαστημοπλοίου ώστε να φτάσει στο πάνω μέρος της οθόνης σε ΤOTAL_MOVING_SECONDS
    float speed = (SCREEN_HEIGHT - 200) / TOTAL_MOVING_SECONDS;

    if (seconds_moving < 0) seconds_moving = 0;

    DrawTextureEx(spaceship, (Vector2){3*MID_WIDTH/2-SPACESHIP_WIDTH/2, SCREEN_HEIGHT-200-speed*seconds_moving}, 0.0, 1.5, WHITE);
}

void draw_transition_screen(State state) {
    draw_counter_section(state);
    draw_moving_spaceship(state);
}
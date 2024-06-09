#include "game_over_screen.h"

void draw_game_over_screen(State state) {
    // Σχεδίαση κινούμενου background
    scrolling_back += 0.2f;
    if (scrolling_back >= SCREEN_HEIGHT) scrolling_back = 0;
    DrawTextureEx(background, (Vector2){ 0, scrolling_back }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scrolling_back}, 0.0f, 2.0f, (Color){255,255,255, 70});

    // Σχεδίαση επικεφαλίδας
    Vector2 size = MeasureTextEx(font, "GAME OVER", 60, 0);
    DrawTextEx(font, "GAME OVER", (Vector2){MID_WIDTH - size.x * 0.5, 60}, 60, 0,WHITE);

    // Σχεδίαση κουμπιών για επαναφορά σε άλλη οθόνη
    DrawTextureEx(play_again_button, (Vector2){MID_WIDTH - PLAY_AGAIN_BUTTON_WIDTH * 0.5, 200}, 0.0, 1.3, WHITE);
    DrawTextureEx(home_button, (Vector2){MID_WIDTH - BUTTON_WIDTH * 0.5, 300}, 0.0, 1.3, WHITE);  
    DrawTextureEx(credits_button, (Vector2){MID_WIDTH - BUTTON_WIDTH * 0.5 - 35, 400}, 0.0, 1.3, WHITE);  

    // Σχεδίαση διακοσμητικού διαστημοπλοίου
    DrawTextureEx(spaceship, (Vector2){MID_WIDTH - 1.3*SPACESHIP_WIDTH * 0.5, SCREEN_HEIGHT-100-SPACESHIP_HEIGHT}, 0.0, 1.3, WHITE);  
}
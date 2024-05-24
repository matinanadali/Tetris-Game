#include "game_over_screen.h"

void draw_game_over_screen(State state) {
    scrolling_back += 0.2f;
    if (scrolling_back >= 2*SCREEN_HEIGHT) scrolling_back = 0;
    DrawTextureEx(background, (Vector2){ 0, scrolling_back }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scrolling_back}, 0.0f, 2.0f, (Color){255,255,255, 70});

    Vector2 size = MeasureTextEx(font, "GAME OVER", 60, 0);
    DrawTextEx(font, "GAME OVER", (Vector2){MID_WIDTH - size.x/2, 60}, 60, 0,WHITE);
    DrawTextureEx(play_again_button, (Vector2){MID_WIDTH - PLAY_AGAIN_BUTTON_WIDTH/2, 200}, 0.0, 1.3, WHITE);
    DrawTextureEx(home_button, (Vector2){MID_WIDTH - BUTTON_WIDTH/2, 300}, 0.0, 1.3, WHITE);  
    DrawTextureEx(spaceship, (Vector2){MID_WIDTH - 1.3*SPACESHIP_WIDTH/2, SCREEN_HEIGHT-100-SPACESHIP_HEIGHT}, 0.0, 1.3, WHITE);  
}
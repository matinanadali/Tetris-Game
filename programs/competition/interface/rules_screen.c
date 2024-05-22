#include "rules_screen.h"
#define TEXT_HEIGHT 30
float scroll = 0.0f;
char *rules_text[10] = {"In this game, a spaceship explores the vast space, trying to avoid asteroids", 
                      "hat appear in its path. The goal of the game is for the spaceship to travel",
                      "as far as possible minimizing the number of asteroids it destroys.\n",
                      "1. Each time a new asteroid appears, your score goes up by 1.",
                      "2. If an asteroid is hit by a bullet, your score drops by 10.",
                      "3. If your spaceship crashes into an asteroid, you lose half of your score.",
                      "4. Every time your score reaches a multiple of 100,", "the game's speed increases by 10%.",
                      "Navigate through space, dodge those asteroids, and see how long you can survive",
                      "in this thrilling interstellar adventure!"}
                      ;

void draw_rules_screen(State state) {
    scroll += 0.2f;
    if (scroll >= 2*SCREEN_HEIGHT) scroll = 0;
    DrawTextureEx(background, (Vector2){ 0, scroll }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scroll}, 0.0f, 2.0f, (Color){255,255,255, 70});

    Vector2 text_size = MeasureTextEx(font, "RULES", 50, 0);
    DrawTextEx(font, "RULES", (Vector2){MID_WIDTH - text_size.x / 2, 40}, 50, 0, WHITE);

    // Τυπώνουμε τον σκοπό του παιχνιδιού
    for (int i = 0; i < 3; i++) {
        text_size = MeasureTextEx(font, rules_text[i], 11, 0);
        DrawTextEx(font, rules_text[i], (Vector2){MID_WIDTH - text_size.x / 2, 100 + TEXT_HEIGHT * (i+1)}, 11, 0, WHITE);
    }

    // Τυπώνουμε τους κανόνες του παιχνιδιού, αφηνόντας ένα κενό από τον σκοπό
    for (int i = 3; i < 8; i++) {
        text_size = MeasureTextEx(font, rules_text[i], 11, 0);
        DrawTextEx(font, rules_text[i], (Vector2){MID_WIDTH - text_size.x / 2, 150 + TEXT_HEIGHT * (i+1)}, 11, 0, WHITE);
    }

    // Τυπώνουμε το τελικό μήνυμα προς τον παίκτη
    for (int i = 8; i < 10; i++) {
        text_size = MeasureTextEx(font, rules_text[i], 11, 0);
        DrawTextEx(font, rules_text[i], (Vector2){MID_WIDTH - text_size.x / 2, 200 + TEXT_HEIGHT * (i+1)}, 11, 0, WHITE);
    }

    DrawTextureEx(home_button, (Vector2){MID_WIDTH - BUTTON_WIDTH / 2 - 10, SCREEN_HEIGHT - BUTTON_HEIGHT - 80}, 0.0, 1.3, WHITE);

    
}
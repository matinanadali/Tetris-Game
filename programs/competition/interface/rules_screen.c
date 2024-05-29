#include "rules_screen.h"
#define TEXT_HEIGHT 30 // ύψος γραμμής
// Κείμενο κανόνων
char *rules_text[10] = {"Welcome to Interstellar Adventure! Your mission is to navigate a spaceship", 
                        "through the vast expanse of space, collecting stars and avoiding asteroids.",
                        "Here are the rules to help you on your journey:\n",
                        "1. Each star you collect increases your score by 1 point.",
                        "2. For every 10 stars you collect, you earn",
                        "one more bullet to help clear your path.\n",
                        "3. If your spaceship crashes into an asteroid, you lose one life.",
                        "4. The game ends, when you have lost all of your lives.",
                        "Navigate through space, dodge those asteroids, and see how long you can survive",
                        "in this thrilling interstellar adventure!"};
                      
void draw_rules_screen(State state) {
    // Σχεδίαση κινούμενου background
    scrolling_back += 0.2f;
    if (scrolling_back >= 2*SCREEN_HEIGHT) scrolling_back = 0;
    DrawTextureEx(background, (Vector2){ 0, scrolling_back }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scrolling_back}, 0.0f, 2.0f, (Color){255,255,255, 70});

    // Σχεδίαση επικεφαλίδας
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

    // Σχεδίαση home button
    DrawTextureEx(home_button, (Vector2){MID_WIDTH - BUTTON_WIDTH / 2 - 10, SCREEN_HEIGHT - BUTTON_HEIGHT - 80}, 0.0, 1.3, WHITE);
}
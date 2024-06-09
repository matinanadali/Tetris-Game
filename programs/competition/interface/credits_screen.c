#include "credits_screen.h"
#define TEXT_HEIGHT 35 // ύψος γραμμής
// Κείμενο κανόνων
char *credits_text[11] = {"Created By:", 
                        "Matina Nadali",
                        "",
                        "Graphics and Textures:",
                        "kenney.nl, space-shooter-redux",
                        "pexels.com, Photo by Kai Pilger",
                        "",
                        "Sound:",
                        "soundsnap.com",
                        "",
                        "Thank you for playing!"
                        };
                      
void draw_credits_screen(State state) {
    // Σχεδίαση κινούμενου background
    scrolling_back += 0.2f;
    if (scrolling_back >= SCREEN_HEIGHT) scrolling_back = 0;
    DrawTextureEx(background, (Vector2){ 0, scrolling_back }, 0.0f, 2.0f, (Color){255,255,255, 70});
    DrawTextureEx(background, (Vector2){ 0, -SCREEN_HEIGHT + scrolling_back}, 0.0f, 2.0f, (Color){255,255,255, 70});

    // Σχεδίαση επικεφαλίδας
    Vector2 text_size = MeasureTextEx(font, "CREDITS", 50, 0);
    DrawTextEx(font, "CREDITS", (Vector2){MID_WIDTH - text_size.x * 0.5, 40}, 50, 0, WHITE);

    // Τυπώνουμε το κείμενο
    for (int i = 0; i < 11; i++) {
        text_size = MeasureTextEx(font, credits_text[i], 18, 0);
        DrawTextEx(font, credits_text[i], (Vector2){MID_WIDTH - text_size.x * 0.5, 100 + TEXT_HEIGHT * (i+1)}, 18, 0, WHITE);
    }

    // Σχεδίαση home button
    DrawTextureEx(home_button, (Vector2){MID_WIDTH - BUTTON_WIDTH * 0.5 - 10, SCREEN_HEIGHT - BUTTON_HEIGHT - 80}, 0.0, 1.3, WHITE);
}
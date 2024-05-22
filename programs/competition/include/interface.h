#include "state_competition.h"
#define BUTTON_WIDTH 122
#define BUTTON_HEIGHT 54
#define MID_WIDTH ( SCREEN_WIDTH / 2 )
#define MID_HEIGHT ( SCREEN_HEIGHT / 2 )

extern Rectangle play_button_bounds;
extern Rectangle rules_button_bounds;
extern Texture spaceship;
extern Texture background;
extern Texture play_button;
extern Texture rules_button;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);
#include "raylib.h"
#include "state.h"
#include "ADTList.h"
#include "vec2.h"
#include <math.h>
#include <stdio.h>



// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state);
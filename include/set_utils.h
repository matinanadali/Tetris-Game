#include "ADTSet.h"

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μικρότερη τιμή του set που είναι μεγαλύτερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_greater(Set set, Pointer value);

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μεγαλύτερη τιμή του set που είναι μικρότερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_smaller(Set set, Pointer value);

// Επιστρέφει τον κόμβο με τη μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, αυτόν με την μικρότερη τιμή του set που είναι μεγαλύτερη
// από value. Αν δεν υπάρχει κανένας τότε επιστρέφει NULL.

SetNode set_find_node_eq_or_greater(Set set, Pointer value);

// Επιστρέφει τον κόμβο με τη μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, αυτόν με την μεγαλύτερη τιμή του set που είναι μικρότερη
// από value. Αν δεν υπάρχει κανένας τότε επιστρέφει NULL.

SetNode set_find_node_eq_or_smaller(Set set, Pointer value);

#include <stdlib.h>
#include "set_utils.h"
#include "ADTSet.h"

Pointer set_find_eq_or_greater(Set set, Pointer value) {
    SetNode node = set_find_node_eq_or_greater(set, value);
    if (node == NULL) {
        return NULL;
    } else {
        return set_node_value(set, node);
    }
}

Pointer set_find_eq_or_smaller(Set set, Pointer value) {
   SetNode node = set_find_node_eq_or_smaller(set, value);
    if (node == NULL) {
        return NULL;
    } else {
        return set_node_value(set, node);
    }
}

SetNode set_find_node_eq_or_greater(Set set, Pointer value) {
    SetNode node = set_find_node(set, value);
    // Αν το value δεν υπάρχει στο set
    if (node == SET_EOF) {
        // Αποθηκεύουμε το DestroyFunc του set και το θέτουμε NULL, 
        // ώστε να μην επηρεαστεί η τιμή value που θα εισάγουμε και 
        // έπειτα θα αφαιρέσουμε από το set
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        // Προσθέτουμε την τιμή value
        set_insert(set, value);

        // Βρίσκουμε τον επόμενο κόμβο
        SetNode next_node = set_next(set, set_find_node(set, value));

        // Αφαιρούμε την τιμή value
        set_remove(set, value);
        // Επαναφέρουμε το destroy_value του set στην προηγούμενη τιμή του
        set_set_destroy_value(set, destroy_value);
        return next_node;
    } else {
        return node;    // Ο κόμβος node έχει την τιμή value
    }
    return NULL;
}

SetNode set_find_node_eq_or_smaller(Set set, Pointer value) {
    SetNode node = set_find_node(set, value);
    // Αν το value δεν υπάρχει στο set
    if (node == SET_EOF) {
        // Αποθηκεύουμε το DestroyFunc του set και το θέτουμε NULL, 
        // ώστε να μην επηρεαστεί η τιμή value που θα εισάγουμε και 
        // έπειτα θα αφαιρέσουμε από το set
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        // Προσθέτουμε την τιμή value
        set_insert(set, value);

        // Βρίσκουμε τον προηγούμενο κόμβο
        SetNode previous_node = set_previous(set, set_find_node(set, value));
        
        // Αφαιρούμε την τιμή value
        set_remove(set, value);
        // Επαναφέρουμε το destroy_value του set στην προηγούμενη τιμή του
        set_set_destroy_value(set, destroy_value);
        return previous_node;
    } else {
        return node;     // Ο κόμβος node έχει την τιμή value
    }
    return NULL;
}




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
    
    if (node == SET_EOF) {
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        set_insert(set, value);

        SetNode next_node = set_next(set, set_find_node(set, value));

        set_remove(set, value);
        set_set_destroy_value(set, destroy_value);
        return next_node;
    } else {
        return node;
    }
    return NULL;
}

SetNode set_find_node_eq_or_smaller(Set set, Pointer value) {
    SetNode node = set_find_node(set, value);
    
    if (node == SET_EOF) {
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        set_insert(set, value);

        SetNode previous_node = set_previous(set, set_find_node(set, value));
        
        set_remove(set, value);
        set_set_destroy_value(set, destroy_value);
        return previous_node;
    } else {
        return node;
    }
    return NULL;
}




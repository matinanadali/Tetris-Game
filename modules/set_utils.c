#include <stdlib.h>
#include "set_utils.h"
#include "ADTSet.h"
Pointer set_find_eq_or_greater(Set set, Pointer value) {
    SetNode node = set_find_node(set, value);
    
    if (node == SET_EOF) {
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        set_insert(set, value);

        SetNode next_node = set_next(set, set_find_node(set, value));
        if (next_node == NULL) return NULL;
        Pointer greater_value = set_node_value(set, next_node);

        set_remove(set, value);
        set_set_destroy_value(set, destroy_value);
        return greater_value;
    } else {
        return set_node_value(set, node);
    }
    return NULL;
}

Pointer set_find_eq_or_smaller(Set set, Pointer value) {
    SetNode node = set_find_node(set, value);
    
    if (node == SET_EOF) {
        DestroyFunc destroy_value = set_set_destroy_value(set, NULL);
        set_insert(set, value);

        SetNode previous_node = set_previous(set, set_find_node(set, value));
        if (previous_node == NULL) return NULL;
        Pointer smaller_value = set_node_value(set, previous_node);
        
        set_remove(set, value);
        set_set_destroy_value(set, destroy_value);
        return smaller_value;
    } else {
        return set_node_value(set, node);
    }
    return NULL;
}
#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "set_utils.h"
int compare_ints(Pointer a, Pointer b) {
    int* ia = a;
    int* ib = b;
    return *ia - *ib;       // αρνητικός, μηδέν ή θετικός, ανάλογα με τη διάταξη των a,b
}

int* create_int(int value) {
    int *p = malloc(sizeof(int));
    *p = value;
    return p;
}

void test_set_find_eq_or_greater() {
    Set set = set_create(compare_ints, free);

    //προσθέτουμε στοιχεία στο set
    for (int i = 0; i < 10; i++) {
        set_insert(set, create_int(i));
    }

    //αναζητούμε στο set την τιμή 5
    int value = 5;
    int* value_in_set = set_find_eq_or_greater(set, &value);
    TEST_ASSERT(*value_in_set == 5);

    //αφαιρούμε το 5 από το set και περιμένουμε η συνάρτηση να επιστρέψει 6
    set_remove(set, value_in_set);
    int* greater_value_in_set = set_find_eq_or_greater(set, &value);
    TEST_ASSERT(*greater_value_in_set == 6);

    //αποδέσμευση μνήμης
    set_destroy(set);
}

void test_set_find_eq_or_smaller() {
    Set set = set_create(compare_ints, free);

    //προσθέτουμε στοιχεία στο set
    for (int i = 0; i < 10; i++) {
        set_insert(set, create_int(i));
    }

    //αναζητούμε στο set την τιμή 5
    int value = 0;
    int* value_in_set = set_find_eq_or_smaller(set, &value);
    TEST_ASSERT(*value_in_set == 0);

    //αφαιρούμε το 0 από το set και περιμένουμε η συνάρτηση να επιστρέψει NULL
    set_remove(set, value_in_set);
    int* smaller_value_in_set = set_find_eq_or_smaller(set, &value);
    TEST_ASSERT(smaller_value_in_set == NULL);

    //αποδέσμευση μνήμης
    set_destroy(set);
}
// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_set_find_eq_or_greater", test_set_find_eq_or_greater },
	{ "test_set_find_eq_or_smaller", test_set_find_eq_or_smaller },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};


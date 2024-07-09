#include "linked_list.h"

#define TABLE set_table*

typedef struct set_table
{
    STRING_POINTER* items;
    size_t hashmapSize;
} set_table;

// Method to calculate the DJB33X hash value for a given key
size_t djb33x_hash(const char *key);
// Method to create a new set table with given hashmap size
TABLE new_set_table(const size_t hashmapSize);
// Method for search an item in the set table
STRING_POINTER search_in_set(TABLE table, STRING_POINTER item);
// Method used to insert an item into the set table
STRING_POINTER insert_in_set(TABLE table, STRING_POINTER item);
// Method used to print the set table
void print_set_table(TABLE table);
// Method used to remove an item from the set table
void remove_item_in_set(TABLE table, STRING_POINTER item);
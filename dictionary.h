#include "set.h"
#include <stdbool.h>

#define ITEM dictionary_item*
#define DICTIONARY dictionary*
#define VALUE void*
#define COLLISION_THRESHOLD 3

typedef union value_item {
    char* str_value;
    int int_value;
    char char_value;
    float float_value;
    double double_value;
    bool bool_value;
} value_item;

typedef struct dictionary_item
{
    STRING_POINTER key;
    value_item value;
} dictionary_item;

typedef struct dictionary
{
    ITEM* items;
    size_t hashmapSize;
    size_t collisionAmount;
} dictionary;

// Method used to initialize a new dictionary
DICTIONARY new_dictionary(const size_t hashmapSize);
// Method used to calculate dictionary
int calculate_dictionary_index(DICTIONARY dictionary, STRING_POINTER key);
// Methods that return if value is a specified type
bool is_string(VALUE value);
bool is_int(VALUE value);
bool is_char(VALUE value);
bool is_float(VALUE value);
bool is_double(VALUE value);
bool is_bool(VALUE value);
// Method used to resize dictionary
void resize_dictionary(DICTIONARY dictionary);
// Method used to insert an element in dictionary
ITEM insert_in_dictionary(DICTIONARY dictionary, STRING_POINTER key, VALUE value);
// Method used to print dictionary elements
void print_dictionary(DICTIONARY dictionary);
// Method used to search an element in dictionary
ITEM search_in_dictionary(DICTIONARY dictionary, ITEM item, int index);
// Method used to remove a specified element in dictionary
void remove_item_in_dictionary(DICTIONARY dictionary, ITEM item);
// Method used to get the value of a specified key in dictionary
value_item* get_dictionary_value(DICTIONARY dictionary, ITEM item);
#include "common_includes.h"

#define STRING_POINTER list_string_item*
#define POINTER_TO_POINTER(x) &x

// Definition of the list_string_item structure in linked list
typedef struct list_string_item
{
    struct list_string_item* previous;
    struct list_string_item* next;
    const char* content;
} list_string_item;

// Method to create new item for the list of strings
STRING_POINTER new_string_list_item(const char* string);
// Method to get the tail of linked list
STRING_POINTER get_list_tail(STRING_POINTER head);
// Method to append an item to the linked list
STRING_POINTER list_append(STRING_POINTER* head, STRING_POINTER* tail, STRING_POINTER newItem);
// Method to print list
void print_list(STRING_POINTER head);
// Method to pop the first item from the linked list
STRING_POINTER pop_first_list_item(STRING_POINTER* head, STRING_POINTER* tail);
// Method to pop the last item from the linked list
STRING_POINTER pop_last_list_item(STRING_POINTER* head, STRING_POINTER* tail);
// Method to remove a specific item from the linked list
void remove_item_in_list(STRING_POINTER* head, STRING_POINTER targetItem);
// Method to insert an item after another item in list
void list_insert_after(STRING_POINTER* head, STRING_POINTER itemToFind, STRING_POINTER itemToInsert);
// Method to insert an item before another item in list
void list_insert_before(STRING_POINTER* head, STRING_POINTER itemToFind, STRING_POINTER itemToInsert);
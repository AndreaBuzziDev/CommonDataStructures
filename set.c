#include "set.h"

#pragma region "METHODS"

// Method to calculate the DJB33X hash value for a given key
size_t djb33x_hash(const char* key)
{
    size_t hash = 5381;

    for(size_t i = 0; i < strlen(key); i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

// Method to create a new set table with given hashmap size
TABLE new_set_table(const size_t hashmapSize)
{
    // Check hashmap size
    if(!hashmapSize)
    {
        fprintf(stderr, "Error! Cannot create a table of size 0!\n");
        return NULL;
    }

    // Allocate memory for table
    TABLE table = malloc(sizeof(set_table));

    if(!table)
    {
        fprintf(stderr, "Error! There's not enough memory for table!\n");
        return NULL;
    }

    // Allocate memory for the nodes array in the table
    table->hashmapSize = hashmapSize;
    table->items = calloc(table->hashmapSize, sizeof(STRING_POINTER));

    if(!table->items)
    {
        free(table);
        fprintf(stderr, "Error! There's not enough memory for array!\n");
        return NULL;
    }

    return table;
}

// Method for search an item in the set table
STRING_POINTER search_in_set(TABLE table, STRING_POINTER item)
{
    if(!table)
    {
        fprintf(stderr, "Error! Trying to access but table is empty!\n");
        return NULL;
    }

    const size_t key = strlen(item->content);

    if(!item->content || !key)
    {
        fprintf(stderr, "Error! Searching for an empty value!\n");
        return NULL;
    }

    const size_t hash = djb33x_hash(item->content);
    const size_t index = hash % table->hashmapSize;

    STRING_POINTER currentItem = table->items[index];

    if(!currentItem)
    {
        fprintf(stderr, "Error! Item named %s not found!\n", item->content);
        return NULL;
    }

    STRING_POINTER previousItem = NULL;
    currentItem = search_in_list(currentItem, &previousItem, item);

    if(currentItem)
    {
        printf("Key %s been found at index %zu position\n", item->content, index);
        return currentItem;
    }

    fprintf(stderr, "Error! Item named %s not found!\n", item->content);

    return NULL;
}

// Method used to insert an item into the set table
STRING_POINTER insert_in_set(TABLE table, STRING_POINTER item)
{
    if(!table)
    {
        fprintf(stderr, "Error! Trying to access but table is empty!\n");
        return NULL;
    }

    const size_t key = strlen(item->content);

    if(!item->content || !key)
    {
        fprintf(stderr, "Error! Trying to insert but table is empty!\n");
        return NULL;
    }

    // Calculate the hash value and index for the item
    size_t hash = djb33x_hash(item->content);
    size_t index = hash % table->hashmapSize;

    STRING_POINTER head = table->items[index];

    // If list is empty at this index, create a new node and set it as head
    if(!head)
    {
        table->items[index] = malloc(sizeof(list_string_item));

        if(!table->items[index])
        {
            fprintf(stderr, "Error! There's not enough memory!\n");
            return NULL;
        }

        table->items[index]->content = item->content;
        table->items[index]->next = NULL;

        return table->items[index];
    }

    // If the head contains the same key print an error
    if(strcmp(head->content, item->content) == 0)
    {
        fprintf(stderr, "Error! Trying to insert an existing key: '%s' found at index %zu!\n", item->content, index);
        return NULL;
    }

    // If the head has a next node, check the rest of the list for duplicates
    if(head->next)
    {
        STRING_POINTER currentItem = head->next;
        STRING_POINTER previousItem = head;

        currentItem =  search_in_list(currentItem, &previousItem, item);

        if(currentItem)
        {
            fprintf(stderr, "Error! Trying to insert an existing key: '%s' found at index %zu!\n", item->content, index);
            return NULL;
        }
    }

    // If there's not duplicates append item to the end of linked list
    list_append(POINTER_TO_POINTER(head), item);

    return item;
}

// Method used to print the set table
void print_set_table(TABLE table)
{
    if(!table)
    {
        fprintf(stderr, "Error! Trying to print but table is empty!\n");
        return;
    }

    for(int i = 0; i < table->hashmapSize; i++)
    {
        if(!table->items[i])
        {
            continue;
        }

        print_list(table->items[i]);
    }

    return;
}

// Method used to remove an item from the set table
void remove_item_in_set(TABLE table, STRING_POINTER item)
{
    if(!table)
    {
        fprintf(stderr, "Error! Trying to remove item but table is empty!\n");
        return;
    }

    const size_t key = strlen(item->content);

    if(!item->content || !key)
    {
        fprintf(stderr, "Error! Trying to remove an item but is empty!\n");
        return;
    }

    const size_t hash = djb33x_hash(item->content);
    const size_t index = hash % table->hashmapSize;

    STRING_POINTER currentItem = table->items[index];

    if(!currentItem)
    {
        fprintf(stderr, "Error! Item named %s not found!\n", item->content);
        return;
    }

    remove_item_in_list(POINTER_TO_POINTER(table->items[index]), item);

    return;
}

#pragma endregion

int main(int argc, char** argv, char** envs)
{
    // Create a new set table with size of 3
    TABLE table = new_set_table(3);

    // Some new items to add to table
    STRING_POINTER item01 = new_string_list_item("NINTENDO64");
    STRING_POINTER item02 = new_string_list_item("PS1");
    STRING_POINTER item03 = new_string_list_item("PS2");
    STRING_POINTER item04 = new_string_list_item("WII");
    STRING_POINTER item05 = new_string_list_item("PS3");
    STRING_POINTER item06 = new_string_list_item("PS4");
    STRING_POINTER item07 = new_string_list_item("PS5");

    printf("----POPULATE-TABLE-----\n");

    // Insert items into the set table
    insert_in_set(table, item01);
    insert_in_set(table, item03);
    insert_in_set(table, item04);
    insert_in_set(table, item05);
    insert_in_set(table, item06);
    insert_in_set(table, item07);

    print_set_table(table);

    printf("\n");

    // Attempt to insert item which value is already present in set table
    printf("---TEST-INSERT-ERROR---\n");

    insert_in_set(table, item04);

    printf("\n");

    printf("---TEST-INSERT---\n");

    printf("Item: '%s' insert ahead\n", insert_in_set(table, item02)->content);

    print_set_table(table);

    printf("\n");

    // Search for items in the set table and display messages
    printf("---TEST-SEARCH---\n");

    search_in_set(table, item06);
    search_in_set(table, item01);

    printf("\n");

    // Remove items from the set table and display messages
    printf("---TEST-REMOVE---\n");

    remove_item_in_set(table, item06);
    remove_item_in_set(table, item05);
    remove_item_in_set(table, item01);
    remove_item_in_set(table, item02);
    
    print_set_table(table);

    return 0;
}
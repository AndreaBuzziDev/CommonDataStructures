#include "linked_list.h"

#pragma region "METHODS"

// Method to create new item for the list of strings
STRING_POINTER new_string_list_item(const char* string)
{
    STRING_POINTER item = malloc(sizeof(list_string_item));

    if(!item)
    {
        fprintf(stderr, "Error! There's not enough memory!\n");
        return NULL;
    }

    item->content = string;

    return item;
}

// Method to get the tail of linked list
STRING_POINTER get_list_tail(STRING_POINTER head)
{
    STRING_POINTER currentItem = head;
    STRING_POINTER lastItem = NULL;

    while(currentItem)
    {
        lastItem = currentItem;
        currentItem = currentItem->next;
    }

    return lastItem;
}

// Method to append an item to the linked list
STRING_POINTER list_append(STRING_POINTER* head, STRING_POINTER newItem)
{
    STRING_POINTER currentItem = get_list_tail(*head);

    if(!currentItem)
    {
        *head = newItem;
    }
    else
    {
        currentItem->next = newItem;
    }

    newItem->next = NULL;

    return newItem;
}

// Method to print list
void print_list(STRING_POINTER head)
{
    STRING_POINTER currentItem = head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Cannot print an empty list!\n");
        return;
    }

    while(currentItem)
    {
        printf("Address: %p Item: %s\n", currentItem, currentItem->content);
        currentItem = currentItem->next;
    }

    return;
}

// Method to pop the first item from the linked list
STRING_POINTER pop_first_list_item(STRING_POINTER* head)
{
    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to pop an item but the list is empty!\n");
        return NULL;
    }
    
    printf("The item '%s' has been popped from the list!\n", currentItem->content);

    *head = currentItem->next;
    currentItem->next = NULL;

    return currentItem;
}

// Method to pop the last item from the linked list
STRING_POINTER pop_last_list_item(STRING_POINTER* head)
{
    STRING_POINTER currentItem = *head;
    STRING_POINTER previousItem = NULL;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to pop an item but the list is empty!\n");
        return NULL;
    }

    if(!currentItem->next)
    {
        *head = NULL;
        printf("The item '%s' has been popped from the list!\n", currentItem->content);

        return currentItem;
    }

    while(currentItem->next)
    {
        previousItem = currentItem;
        currentItem = currentItem->next;
    }

    previousItem->next = NULL;
    printf("The item '%s' has been popped from the list!\n", currentItem->content);

    return currentItem;
}

// Method to search for a specific item in the linked list
STRING_POINTER search_in_list(STRING_POINTER currentItem, STRING_POINTER* previousItem, STRING_POINTER targetItem)
{
    while(currentItem)
    {
        if((currentItem == targetItem) || (strcmp(currentItem->content, targetItem->content) == 0))
        {
            return currentItem;
        }

        *previousItem = currentItem;
        currentItem = currentItem->next;
    }

    return NULL;
}

// Method to remove a specific item from the linked list
void remove_item_in_list(STRING_POINTER* head, STRING_POINTER targetItem)
{
    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to remove an item but the list is empty!\n");
        return;
    }

    STRING_POINTER previousItem = NULL;
    currentItem = search_in_list(currentItem, &previousItem, targetItem);

    if(!currentItem)
    {
        fprintf(stderr, "Error! The given item cannot be found!\n");
        return;
    }

    if(previousItem)
    {
        previousItem->next = currentItem->next;
    }
    else
    {
        *head = currentItem->next;
    }

    printf("Item '%s' has been removed!\n", targetItem->content);

    free(currentItem);

    return;
}

#pragma endregion

int main(int argc, char** argv, char** envs)
{
    // Empty linked list init
    STRING_POINTER head = NULL;

    // Some new items to add to list
    STRING_POINTER item01 = new_string_list_item("First Item");
    STRING_POINTER item02 = new_string_list_item("Second Item");
    STRING_POINTER item03 = new_string_list_item("Third Item");

    printf("------POPULATE-LIST----\n");
    
    // Append items to list
    list_append(POINTER_TO_POINTER(head), item01);
    list_append(POINTER_TO_POINTER(head), item02);
    list_append(POINTER_TO_POINTER(head), item03);

    print_list(head);

    printf("\n");

    printf("------MANIPULATE-LIST----\n");
    pop_last_list_item(POINTER_TO_POINTER(head));
    STRING_POINTER item04 = new_string_list_item("New Item");
    list_append(POINTER_TO_POINTER(head), item04);
    remove_item_in_list(POINTER_TO_POINTER(head), item01);

    free(item03);

    return 0;
}
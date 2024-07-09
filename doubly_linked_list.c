#include "doubly_linked_list.h"

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
STRING_POINTER list_append(STRING_POINTER* head, STRING_POINTER* tail, STRING_POINTER newItem)
{
    if(!newItem)
    {
        fprintf(stderr, "Error! Trying to append NULL item!\n");
        return NULL;
    }

    if(!(*tail))
    {
        *head = newItem;
        *tail = newItem;
    }
    else
    {
        (*tail)->next = newItem;
    }

    newItem->previous = *tail;
    newItem->next = NULL;
    *tail = newItem;

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
STRING_POINTER pop_first_list_item(STRING_POINTER* head, STRING_POINTER* tail)
{
    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to pop an item but the list is empty!\n");
        return NULL;
    }

    *head = (*head)->next;

    if(*head)
    {
        (*head)->previous = NULL;
    }
    else
    {
        *tail = NULL;
    }

    printf("The item '%s' has been popped from the list!\n", currentItem->content);

    currentItem->previous = NULL;
    currentItem->next = NULL;

    return currentItem;
}

// Method to pop the last item from the linked list
STRING_POINTER pop_last_list_item(STRING_POINTER* head, STRING_POINTER* tail)
{
    STRING_POINTER currentItem = *tail;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to pop an item but the list is empty!\n");
        return NULL;
    }

    if(currentItem->previous)
    {
        (*tail)->previous->next = NULL;
    }
    else
    {
        *head = NULL;
    }

    (*tail) = currentItem->previous;

    printf("The item '%s' has been popped from the list!\n", currentItem->content);

    currentItem->previous = NULL;
    currentItem->next = NULL;

    return currentItem;
}

// Method to remove a specific item from the linked list
void remove_item_in_list(STRING_POINTER* head, STRING_POINTER targetItem)
{
    if(!targetItem)
    {
        fprintf(stderr, "Error! Cannot use a NULL item!\n");
        return;
    }

    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Trying to remove an item but the list is empty!\n");
        return;
    }

    if(currentItem == targetItem)
    {
        *head = currentItem->next;

        if(*head)
        {
            (*head)->previous = NULL;
        }

        free(currentItem);

        return;
    }

    while(currentItem)
    {
        if(currentItem == targetItem)
        {
            currentItem->previous->next = currentItem->next;

            if(currentItem->next)
            {
                currentItem->next->previous = currentItem->previous;
            }

            printf("Item '%s' has been removed!\n", targetItem->content);

            free(currentItem);

            return;
        }

        currentItem = currentItem->next;
    }

    fprintf(stderr,  "Error! The given item cannot be found!\n");

    return;
}

// Method to insert an item after another item in list
void list_insert_after(STRING_POINTER* head, STRING_POINTER itemToFind, STRING_POINTER itemToInsert)
{
    if(!itemToFind)
    {
        fprintf(stderr, "Error! Cannot find a NULL item!\n");
        return;
    }

    if(!itemToInsert)
    {
        fprintf(stderr, "Error! Cannot insert a NULL item!\n");
        return;
    }

    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Cannot insert item into an empty list!\n");
        return;
    }

    while(currentItem)
    {
        if(currentItem == itemToFind)
        {
            if(currentItem->next)
            {
                currentItem->next->previous = itemToInsert;
                itemToInsert->next = currentItem->next;
                itemToInsert->previous = currentItem;
                currentItem->next = itemToInsert;
            }
            else
            {
                currentItem->next = itemToInsert;
                itemToInsert->previous = currentItem;
                itemToInsert->next = NULL;
            }

            printf("The item '%s' has been inserted after '%s'\n", itemToInsert->content, itemToFind->content); 

            return;
        }

        currentItem = currentItem->next;
    }


    fprintf(stderr, "Error! The given item cannot be found!\n");

    return;
}

// Method to insert an item before another item in list
void list_insert_before(STRING_POINTER* head, STRING_POINTER itemToFind, STRING_POINTER itemToInsert)
{
    if(!itemToFind)
    {
        fprintf(stderr, "Error! Cannot find a NULL item!\n");
        return;
    }

    if(!itemToInsert)
    {
        fprintf(stderr, "Error! Cannot insert a NULL item!\n");
        return;
    }

    STRING_POINTER currentItem = *head;

    if(!currentItem)
    {
        fprintf(stderr, "Error! Cannot insert item into an empty list!\n");
        return;
    }

    if(currentItem == itemToFind)
    {
        itemToInsert->previous = NULL;
        itemToInsert->next = currentItem;
        currentItem->previous = itemToInsert;
        *head = itemToInsert;

        printf("The item '%s' has been inserted before '%s'\n", itemToInsert->content, itemToFind->content); 

        return;
    }

    while(currentItem)
    {
        if(currentItem == itemToFind)
        {
            itemToInsert->previous = currentItem->previous;
            itemToInsert->next = currentItem;
            currentItem->previous->next = itemToInsert;
            currentItem->previous = itemToInsert;

            printf("The item '%s' has been inserted before '%s'\n", itemToInsert->content, itemToFind->content); 

            return;
        }

        currentItem = currentItem->next;
    }

    fprintf(stderr, "Error! The given item cannot be found!\n");

    return;
}

#pragma endregion

int main(int argc, char** argv, char** envs)
{
    // Create and initialize the first linked list (head01)
    printf("----------FIRST-LIST----------\n");

    STRING_POINTER head01 = NULL;
    STRING_POINTER tail01 = NULL;

    // Create string items for the first linked list
    STRING_POINTER item01 = new_string_list_item("First Item");
    STRING_POINTER item02 = new_string_list_item("Second Item");
    STRING_POINTER item03 = new_string_list_item("Third Item");
    STRING_POINTER item04 = new_string_list_item("Fourth Item");

    // Append items to the first linked list
    list_append(POINTER_TO_POINTER(head01), POINTER_TO_POINTER(tail01), item01);
    list_append(POINTER_TO_POINTER(head01), POINTER_TO_POINTER(tail01), item02);
    list_append(POINTER_TO_POINTER(head01), POINTER_TO_POINTER(tail01), item03);
    list_append(POINTER_TO_POINTER(head01), POINTER_TO_POINTER(tail01), item04);

    // Print the first linked list
    print_list(head01);

    // Print the first linked list after pop and remove calls
    pop_last_list_item(POINTER_TO_POINTER(head01), POINTER_TO_POINTER(tail01));
    remove_item_in_list(POINTER_TO_POINTER(head01), item02);
    print_list(head01);

    // Create and initialize the second linked list (head02)
    printf("----------SECOND-LIST----------\n");

    STRING_POINTER head02 = NULL;
    STRING_POINTER tail02 = NULL;

    // Create string items for the first linked list
    STRING_POINTER item05 = new_string_list_item("GodOfWar");
    STRING_POINTER item06 = new_string_list_item("CallOfDuty");
    STRING_POINTER item07 = new_string_list_item("HellBlade");
    STRING_POINTER item08 = new_string_list_item("FIFA16");

    // Append items to the first linked list
    list_append(POINTER_TO_POINTER(head02), POINTER_TO_POINTER(tail02), item05);
    list_append(POINTER_TO_POINTER(head02), POINTER_TO_POINTER(tail02), item06);
    print_list(head02);

    list_insert_after(POINTER_TO_POINTER(head02), item05, item07);
    list_insert_before(POINTER_TO_POINTER(head02), item05, item08);
    print_list(head02);
}
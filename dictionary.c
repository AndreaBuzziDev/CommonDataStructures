#include "dictionary.h"

DICTIONARY new_dictionary(const size_t hashmapSize)
{
    if(!hashmapSize)
    {
        fprintf(stderr, "Error! Cannot create a table of 0 size!\n");
        return NULL;
    }

    DICTIONARY dictionary = malloc(sizeof(dictionary));

    if(!dictionary)
    {
        fprintf(stderr, "Error! There's not enough memory for dictionary!\n");
        return NULL;
    }

    dictionary->hashmapSize = hashmapSize;
    dictionary->items = calloc(dictionary->hashmapSize, sizeof(ITEM));

    if(!dictionary->items)
    {
        free(dictionary);
        fprintf(stderr, "Error! There's not enough memory for items!\n");
        return NULL;
    }

    dictionary->collisionAmount = 0;

    return dictionary;
}

int calculate_dictionary_index(DICTIONARY dictionary, STRING_POINTER key)
{
    const size_t keyLen = strlen(key->content);

    if(!key->content || !keyLen)
    {
        return -1;
    }

    size_t hash = djb33x_hash(key->content);
    int index = hash % dictionary->hashmapSize;

    return index;
}

bool is_string(VALUE value)
{
    return (sizeof(*(char**)value) == sizeof(char*));
}

bool is_int(VALUE value)
{
    return (sizeof(*(int*)value) == sizeof(int));
}

bool is_char(VALUE value)
{
    return (sizeof(*(char*)value) == sizeof(char));
}

bool is_float(VALUE value)
{
    return (sizeof(*(float*)value) == sizeof(float));
}

bool is_double(VALUE value)
{
    return (sizeof(*(double*)value) == sizeof(double));
}

bool is_bool(VALUE value)
{
    return (sizeof(*(bool*)value) == sizeof(bool));
}

void resize_dictionary(DICTIONARY dictionary)
{
    size_t oldHashmapSize = dictionary->hashmapSize;
    dictionary->hashmapSize = (dictionary->hashmapSize * 2);

    ITEM* newItems = calloc(dictionary->hashmapSize, sizeof(ITEM));

    if(!newItems)
    {
        free(dictionary);
        fprintf(stderr, "Error! There's not enough memory for newItems!\n");
    }

    for (size_t i = 0; i < oldHashmapSize; i++)
    {
        if(dictionary->items[i] && dictionary->items[i]->key)
        {
            STRING_POINTER currentItem = dictionary->items[i]->key;

            while(currentItem)
            {
                STRING_POINTER poppedItem = pop_last_list_item(&currentItem);

                int index = calculate_dictionary_index(dictionary, poppedItem);
                
                if(newItems[index]->key)
                {
                    list_append(POINTER_TO_POINTER(newItems[index]->key), poppedItem);
                }
                else
                {
                    newItems[index] = dictionary->items[i];
                    newItems[index]->key = poppedItem;
                    newItems[index]->value = dictionary->items[i]->value;
                }
            }
        }

    }

    return;
}

ITEM insert_in_dictionary(DICTIONARY dictionary, STRING_POINTER key, VALUE value)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error! Trying to access dictionary but is empty!\n");
        return NULL;
    }

    int index = dictionary_calculate_index(dictionary, key);

    if(index < 0)
    {
        fprintf(stderr, "Error! Cannot insert a key that has no value!\n");
        return NULL;
    }

    ITEM currentItem = dictionary->items[index];

    if(!currentItem)
    {
        dictionary->items[index] = malloc(sizeof(dictionary_item));

        if(!dictionary->items[index])
        {
            fprintf(stderr, "Error! There's not enough memory for items!\n");
            return NULL;
        }
        
        dictionary->items[index]->key = new_string_list_item(key->content);

        if(!dictionary->items[index]->key)
        {
            fprintf(stderr, "Error! There's not enough memory for stringItem!\n");
            free(dictionary->items[index]);

            return NULL;
        }

        dictionary->items[index]->key->content = key->content;
        
        if(is_string(value))
        {
            dictionary->items[index]->value.str_value = (char*)value;
        }
        else if(is_int(value))
        {
            dictionary->items[index]->value.int_value = *(int*)value;
        }
        else if(is_char(value))
        {
            dictionary->items[index]->value.char_value = *(char*)value;
        }
        else if(is_float(value))
        {
            dictionary->items[index]->value.float_value = *(float*)value;
        }
        else if(is_double(value))
        {
            dictionary->items[index]->value.double_value = *(double*)value;
        }
        else if(is_bool(value))
        {
            dictionary->items[index]->value.bool_value = *(bool*)value;
        }
        else
        {
            fprintf(stderr, "Error! Value type not supported!\n");
            free(dictionary->items[index]->key);
            free(dictionary->items[index]);

            return NULL;
        }

        return dictionary->items[index];
    }
    else
    {
        dictionary->collisionAmount = (dictionary->collisionAmount + 1);
    }

    if(strcmp(currentItem->key->content, key->content) == 0)
    {
        fprintf(stderr, "Error! Trying to insert an existing key: '%s', found at index '%d'!\n", key->content, index);
        return NULL;
    }
    
    if(currentItem->key->next)
    {
        STRING_POINTER currentKey = currentItem->key->next;
        STRING_POINTER previousKey = currentItem->key;

        currentKey = search_in_list(currentKey, &previousKey, key);

        if(currentKey)
        {
            fprintf(stderr, "Error! Trying to insert an existing key: %s found at index %d!\n", currentKey->content, index);
            return NULL;
        }
    }

    ITEM newItem = malloc(sizeof(dictionary_item));

    if(!newItem)
    {
        fprintf(stderr, "Error! There's not enough memory for newItem!\n");
        return NULL;
    }
    
    STRING_POINTER newListStringItem = new_string_list_item(key->content);

    if(!newListStringItem)
    {
        fprintf(stderr, "Error! Cannot allocate enough memory for newListStringItem!\n");
        return NULL;
    }

    newItem->key = newListStringItem;
    newItem->key->content = key->content;
    
    if(is_string(value))
    {
        newItem->value.str_value = (char*)value;
    }
    else if(is_int(value))
    {
        newItem->value.int_value = *(int*)value;
    }
    else if(is_char(value))
    {
        newItem->value.char_value = *(char*)value;
    }
    else if(is_float(value))
    {
        newItem->value.float_value = *(float*)value;
    }
    else if(is_double(value))
    {
        newItem->value.double_value = *(double*)value;
    }
    else if(is_bool(value))
    {
        newItem->value.bool_value = *(bool*)value;
    }
    else
    {
        fprintf(stderr, "Error! Value type not supported!\n");
        free(newItem->key);
        free(newItem);
        return NULL;
    }

    list_append(POINTER_TO_POINTER(currentItem->key), newListStringItem);

    return newItem;
}

void print_dictionary(DICTIONARY dictionary)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error! Trying to print dictionary but is empty!\n");
        return;
    }

    for(int i = 0; i < dictionary->hashmapSize; i++)
    {
        if(!dictionary->items[i])
        {
            continue;
        }

        printf("Index: %d\n", i+1);
        list_print(dictionary->items[i]->key);
    }

    return;
}

ITEM search_in_dictionary(DICTIONARY dictionary, ITEM item, int index)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error! TTrying to print dictionary but is empty!\n");
        return NULL;
    }

    if(index < 0)
    {
        index = calculate_dictionary_index(dictionary, item->key);
    }

    if(index < 0)
    {
        fprintf(stderr, "Error! Cannot insert a key that has no value!\n");
        return NULL;
    }

    ITEM currentItem = dictionary->items[index];

    if(!currentItem)
    {
        fprintf(stderr, "Error! Item named '%s' not found!\n", item->key->content);
        return NULL;
    }

    STRING_POINTER currentKey = currentItem->key;

    if(!currentKey)
    {
        fprintf(stderr, "Error! Key named '%s' not found!\n", item->key->content);
        return NULL;
    }

    STRING_POINTER previousItem = NULL;

    currentKey = search_in_list(currentKey, &previousItem, item->key);

    if(currentKey)
    {
        printf("Key '%s' found at index '%d'\n", item->key->content, index);
        return item;
    }

    fprintf(stderr, "Error! Item named '%s' not found!\n", item->key->content);

    return NULL;
}

void remove_item_in_dictionary(DICTIONARY dictionary, ITEM item)
{
    const int index = calculate_dictionary_index(dictionary, item->key);

    if(index < 0)
    {
        fprintf(stderr, "Error! Cannot insert a key that has no value!\n");
        return;
    }

    ITEM itemToRemove = search_in_dictionary(dictionary, item, index);

    if(itemToRemove)
    {
        remove_item_in_list(POINTER_TO_POINTER(dictionary->items[index]->key), itemToRemove->key);

        free(itemToRemove);
    }

    return;
}

value_item* get_dictionary_value(DICTIONARY dictionary, ITEM item)
{
    ITEM itemFound = search_in_dictionary(dictionary, item, -1);
    
    if(itemFound)
    {
        return &(item->value);
    }

    return NULL;
}

int main(int argc, char** argv, char** envs)
{
    DICTIONARY dictionary = new_dictionary(3);

    STRING_POINTER key1 = new_string_list_item("PLAYSTATION");
    STRING_POINTER key2 = new_string_list_item("ABITANTIMOLISE");
    STRING_POINTER key3 = new_string_list_item("PAYRESPECT");
    STRING_POINTER key4 = new_string_list_item("PROSPECTCREEK");
    STRING_POINTER key5 = new_string_list_item("CONTOINBANCA");
    STRING_POINTER key6 = new_string_list_item("THATSENOUGH");
    STRING_POINTER key10 = new_string_list_item("TESTERROR");

    printf("---POPULATE-DICTIONARY---\n");

    ITEM item1 = dictionary_insert(dictionary, key1, (VALUE)"SONY");
    ITEM item2 = dictionary_insert(dictionary, key2, (VALUE)305617);
    ITEM item3 = dictionary_insert(dictionary, key3, (VALUE)'F');
    ITEM item4 = dictionary_insert(dictionary, key4, (VALUE)-62);
    ITEM item5 = dictionary_insert(dictionary, key5, (VALUE)-50000);
    ITEM item6 = dictionary_insert(dictionary, key6, (VALUE)false);
    ITEM item7 = NULL;
    ITEM item10 = NULL;

    print_dictionary(dictionary);

    printf("\n");

    printf("---RESIZE-DICTIONARY---\n");

    resize_dictionary(dictionary);
    print_dictionary(dictionary);

    printf("\n");
    
    printf("---ERRORS-TEST---\n");

    insert_in_dictionary(dictionary, key3, (VALUE)234);
    VALUE valueTest1 = get_dictionary_value(dictionary, item7);
    remove_item_in_dictionary(dictionary, item10);

    printf("\n");
    
    printf("---SEARCH-IN-TEST---\n");

    search_in_dictionary(dictionary, item2, -1);
    search_in_dictionary(dictionary, item4, -1);

    printf("\n");
    
    printf("---GET-VALUE-TEST---\n");

    printf("Key: %s, Value: %s\n",item1->key->content, *(char**)get_dictionary_value(dictionary, item1));
    printf("Key: %s, Value: %d\n",item2->key->content, *(int*)get_dictionary_value(dictionary, item2));

    printf("\n");
    
    printf("---REMOVE---\n");

    remove_item_in_dictionary(dictionary, item4);
    remove_item_in_dictionary(dictionary, item6);

    return 0;
}
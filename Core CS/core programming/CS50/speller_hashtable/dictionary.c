/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// link list structure
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

#define HASH_MAX 500
node *hashtable[HASH_MAX] = {NULL};

int hash(const char *word);
char *to_lowercase(const char *word);


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    char *lowercase = to_lowercase(word);
    node *cursor = hashtable[hash(lowercase)];
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, lowercase) == 0)
        {
            free(lowercase);
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    int index;
    char word[LENGTH + 1];
    FILE *fp = fopen(dictionary, "r");

    while (fscanf(fp, "%s", word) != EOF)
    {
        index = hash(word);

        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);
        new_node->next = hashtable[index];
        hashtable[index] = new_node;
    }

    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    unsigned int count = 0;
    node *cursor = NULL;

    for (int i = 0; i < HASH_MAX; i++)
    {
        cursor = hashtable[i];

        while (cursor != NULL)
        {
            count++;
            cursor = cursor->next;
        }
    }

    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    node *cursor = NULL;
    for (int i = 0; i < HASH_MAX; i++)
    {
        cursor = hashtable[i];
        if (cursor == NULL)
        {
            return false;
        }
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

char *to_lowercase(const char *word)
{
    int length = strlen(word);
    char *lowercase = malloc(sizeof(char) *(length + 1));
    strcpy(lowercase, word);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(lowercase[i]) && (lowercase[i] - 'A' <= 26))
        {
            lowercase[i] = tolower(lowercase[i]);
        }
    }
    return lowercase;
}

int hash(const char *word)
{
    int sum = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += word[i];
    }
    return sum % HASH_MAX;
}
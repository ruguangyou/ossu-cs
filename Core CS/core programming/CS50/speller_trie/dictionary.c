/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// trie structure
typedef struct trie
{
    bool is_word;
    struct trie *next[27];
}
trie;

trie *root = NULL;
trie *temp = NULL;

char *to_lowercase(const char *word);

unsigned int count = 0;
int flag = 0;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    temp = root;
    int index;
    char *lowercase = to_lowercase(word);

    for (int i = 0, n = strlen(lowercase); i < n; i++)
    {
        index = (lowercase[i] == '\'') ? 26 : (lowercase[i] - 'a');
        if (temp->next[index] == NULL)
        {
            free(lowercase);
            return false;
        }
        temp = temp->next[index];
    }

    free(lowercase);
    return (temp->is_word == true) ? true : false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    root = malloc(sizeof(trie));
    if (root == NULL)
    {
        return false;
    }
    *root = (trie) {false, {NULL}};

    int index;

    char word[LENGTH + 1];
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }

    while (fscanf(fp, "%s", word) != EOF)
    {
        temp = root;
        // for every word, find its path
        for (int i = 0, n = strlen(word); i < n; i++)
        {
            index = (word[i] == '\'') ? 26 : (word[i] - 'a');

            if (temp->next[index] == NULL)
            {
                temp->next[index] = malloc(sizeof(trie));
                // check if malloc success
                if (temp->next[index] == NULL)
                {
                    unload();
                    return false;
                }
            }
            temp = temp->next[index];
        }

        temp->is_word = true;
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
    if (flag == 0)
    {
        temp = root;
    }
    flag = 1;

    trie *trav = temp;
    for (int i = 0; i < 27; i++)
    {
        if (trav->next[i] != NULL)
        {
            temp = trav->next[i];
            if (size() == 1)
            {
                count++;
            }
        }
    }

    if (trav->is_word == true)
    {
        return 1;
    }
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload()
{
    // TODO
    if (flag == 1)
    {
        temp = root;
    }
    flag = 0;

    trie *trav = temp;
    for (int i = 0; i < 27; i++)
    {
        if (trav->next[i] != NULL)
        {
            temp = trav->next[i];
            unload();
        }
    }
    free(trav);

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
// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // allocate memory for word buffer
    char *str = malloc(LENGTH);
    if (str == NULL)
    {
        printf("Could not allocate memory for word buffer\n");
        return false;
    }
    
    // open file to read words
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s", dictionary);
        return false;
    }
    printf("Opened dictionary...\n");

    // take every word, hash it and put in table
    while (fgets(str, LENGTH, fp) != NULL)
    {
        node *newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("Could not allocate memory for new node\n");
            return false;
        }

        int hashedWord = hash(str);

        newNode->next = table[hashedWord]; // set new node to point to first node in list (first time is NULL)
        strcpy(newNode->word, str);

        table[hashedWord] = newNode; // set beginning of list to point to new node (make new node first node in list)
    }
    printf("EOF or error encountered. Ended loading dictionary into memory.\n");
    free(str);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}

// ----------------------------------------------------------------- tools

void print_list_array()
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];

        while (ptr != NULL)
        {
            printf("%s", ptr->word);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    char* name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
int vote(char* name);
void print_winner(void);
candidate random_candidate();

int main(int argc, char* argv[])
{
    srand (time(NULL)); // new seed for rand each time the program runs

    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = 5;
    printf("Number of voters: %i\n", voter_count);

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        char* name = malloc(11);
        name = random_candidate().name;
        printf("Vote: %s\n", name);

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
int vote(char* name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (name = candidates[i].name)
        {
            candidates[i].votes++;
            return 1;
        }
    }
    return 0;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    return;
}

candidate random_candidate(){
    int j = rand() % candidate_count; // picks random num
    return candidates[j];
}
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
void mergesort(candidate* arr, int startIndex, int endIndex);
void merge(candidate* arr, int startIndex, int mid, int endIndex);

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

    int voter_count = 500;
    printf("Number of voters: %i\n", voter_count);

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        char* name;
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
        if (strcmp(name, candidates[i].name) == 0)
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
    printf("Merge sort iterations: ");
    mergesort(candidates, 0, candidate_count - 1);
    printf("\n");
    
    if (candidates[0].votes == candidates[1].votes)
    {
        printf("The winners are:\n");
        for (int i = 0; i < candidate_count; i++)
        {
            if (candidates[i].votes == candidates[0].votes)
            {
                printf("%s\n",candidates[i].name);
            }
            
        }
    }else
    {
        printf("The winner is:\n%s\n", candidates[0].name);
    }
}

candidate random_candidate(){
    int j = rand() % candidate_count; // picks random num
    return candidates[j];
}

// Sort candidades array in order of most votes
void mergesort(candidate* arr, int startIndex, int endIndex)
{
    if (startIndex < endIndex)
    {
        int mid = (startIndex + endIndex) / 2;
        mergesort(arr, startIndex, mid);
        mergesort(arr, mid + 1, endIndex);

        merge(arr, startIndex, mid, endIndex);
    }
}
  
void merge(candidate* arr, int startIndex, int mid, int endIndex)
{
    int i = startIndex, j = mid + 1, tempIndex = 0;
    candidate* temp = malloc(sizeof(candidate) * (endIndex - startIndex + 1));
    int memor = endIndex - startIndex + 1;
    printf("%i ", memor);
    while ((i <= mid) && (j <= endIndex))
    {
        if (candidates[i].votes > candidates[j].votes)
        {
            temp[tempIndex] = arr[i];
            tempIndex++;
            i++;
        }
        else
        {
            temp[tempIndex] = arr[j];
            tempIndex++;
            j++;
        }
    }
    while (j <= endIndex)
    {
        temp[tempIndex] = arr[j];
        tempIndex++;
        j++;
    }
    while (i <= mid)
    {
        temp[tempIndex] = arr[i];
        tempIndex++;
        i++;
    }
  
    for (i = startIndex, tempIndex = 0; i <= endIndex; i++, tempIndex++)
    {
        candidates[i] = temp[tempIndex];
    }
    free(temp);
}
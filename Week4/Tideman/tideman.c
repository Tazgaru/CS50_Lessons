#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
int locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
char *candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
int vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void mergesort(pair* arr, int startIndex, int endIndex);
void merge(pair* arr, int startIndex, int mid, int endIndex);
int make_loop(int init_node, int next_node);
int find_winners(int winners[]);

void print_preferences(); // ----------- tools
void print_pairs(); // ----------- tools
char* random_candidate(); // ----------- tools
void print_locked(); // ----------- tools

int main(int argc, char *argv[])
{
    srand (time(NULL)); // // ----------- tools (new seed for rand each time the program runs)

    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = 0;
        }
    }

    pair_count = 0;
    printf("Number of voters: ");
    int voter_count;
    //scanf("%i", &voter_count);
    voter_count = 1500; //---------------- tools
    printf("%i\n", voter_count);

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            printf("Rank %i: ", j + 1);
            //char *name = malloc(11);
            char *name; // ------------------- tools
            //scanf("%s", name);
            // ------------------------------------------------ tools
            int found = 0;
            do
            {
                name = random_candidate();
                for (int k = 0; k < j; k++)
                {
                    if (strcmp(name, candidates[ranks[k]]) == 0) // Ensure candidate name is not repeated
                    {
                        found = 1;
                        break;
                    }else
                    {
                        found = 0;
                    }
                }
                
            } while (found == 1);
            // ----------------------------------------------------^
            
            
            

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
            //free(name); // ----------------- tools (malloc pointer points to candidates now)
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();


    //print_preferences(); // ------------------------ tool
    //print_pairs(); // ------------------------ tool
    print_locked(); // ------------------------ tool

    print_winner();

    return 0;
}

// Update ranks given a new vote
int vote(int rank, char *name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++) // Iterates over candidates, when find matching names, puts candidate index in ranks array in order of preference
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            printf("%s's index added to ranks array.\n", candidates[i]);
            return 1;
        }
    }
    return 0;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int count = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[count].winner = i;
                pairs[count].loser = j;
                pair_count++;
                count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[count].winner = j;
                pairs[count].loser = i;
                pair_count++;
                count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{   
    mergesort(pairs, 0, pair_count - 1);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (make_loop(pairs[i].winner, pairs[i].loser) == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = 1;
        }
    }
}

// Print the winner of the election
// If there is a tie, print all winners
void print_winner(void)
{
    int winners[candidate_count];
    int numWinners = find_winners(winners);

    if (numWinners == 1)
    {
        printf("Winner is: \n%s\n", candidates[winners[0]]);
    }else if (numWinners > 1)
    {
        printf("Winners are: \n");
        for (int i = 0; i < numWinners; i++)
        {
            printf("%s\n", candidates[winners[i]]);
        }
    }
}

// Merge sort algo using the strenght of victory of each pair
void mergesort(pair* arr, int startIndex, int endIndex)
{
    if (startIndex < endIndex)
    {
        int mid = (startIndex + endIndex) / 2;
        mergesort(arr, startIndex, mid);
        mergesort(arr, mid + 1, endIndex);

        merge(arr, startIndex, mid, endIndex);
    }
}
  
void merge(pair* arr, int startIndex, int mid, int endIndex)
{
    int i = startIndex, j = mid + 1, tempIndex = 0;
    pair* temp = malloc(endIndex - startIndex + 1);
    printf("%i", endIndex - startIndex + 1);
    while ((i <= mid) && (j <= endIndex))
    {
        int firstDif = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        int secondDif = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
        if (firstDif > secondDif)
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
        pairs[i] = temp[tempIndex];
    }
    free(temp);
}

// Return 1 if connecting init_node to next_node create a loop, 0 otherwise
int make_loop(int init_node, int next_node)
{
    int looping = 0;
    if (init_node == next_node) // check if reached initial node
    {
        printf("Found loop\n");
        return 1;
    }
    for (int i = 0; i < candidate_count && looping == 0; i++) // look for connection
    {
        if (locked[next_node][i] == 1) // when find connection, look for connection on next node
        {
            printf("Found connection between %i and %i\n",next_node, i);
            looping = make_loop(init_node, i);
        }
    }
    if (looping == 0)
    {
        printf("Returned 0\n");
        return 0;
    }else
    {
        printf("Returned 1\n");
        return 1;
    }
}

// Add winners to winners array and return number of winners
int find_winners(int winners[])
{
    int winnerIndex = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == 1)
            {
                break;
            }
            else if (j == candidate_count - 1)
            {
                winners[winnerIndex] = i;
                winnerIndex++;
            }   
        }
    }
    return winnerIndex;
}

// ---------------------------- tools --------------------------------------

// Print visualization of preferences array
void print_preferences()
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                printf("Preferences[(%i)%s][(%i)%s] = %i\n", i, candidates[i], j, candidates[j], preferences[i][j]);
            }
        }
        
    }
    
}
// Print visualization of pairs array with difference in preference over each other
void print_pairs()
{
    printf("Pair count: %i\n", pair_count);
    for (int i = 0; i < pair_count; i++)
    {
        printf("Pair: %i\nWinner: (%i)%s\nLoser: (%i)%s\n", i, pairs[i].winner, candidates[pairs[i].winner], pairs[i].loser, candidates[pairs[i].loser]);
        printf("%i vs %i Dif = %i\n", preferences[pairs[i].winner][pairs[i].loser], preferences[pairs[i].loser][pairs[i].winner], preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
    }
}
// Print visualization of locked array
void print_locked()
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
    
}

char* random_candidate(){
    int j = rand() % candidate_count; // picks random num
    return candidates[j];
}

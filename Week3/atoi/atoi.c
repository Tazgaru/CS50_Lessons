#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int convert(char* input);

int main(void)
{
    printf("Enter a positive integer: ");
    char* input = malloc(20);
    scanf("%s", input);


    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
    free(input);
}

int convert(char* input)
{
    int len = strlen(input);
    int answer = 0;
    int digit;
    for (int i = 0; i < len; i++)
    {
        digit = input[i] - '0';
        answer += digit;

        if (i != len - 1)
        {
            answer *= 10;
        }
    }
    return answer;
}
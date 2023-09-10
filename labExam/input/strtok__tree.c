#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_array_from_string(char *line, int* level_order)
{
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL)
    {
        sscanf(token, "%d", &level_order[i]);
        token = strtok(NULL, " ");
        i++;
    }

    // for (j = 0; j < i; j++)
    // {
    //     if (level_order[j] == -2)
    //         level_order[j] = -1;

    //     printf("%d ", level_order[j]);
    // }
    // return j-1;
}

int main(int argc, char const *argv[])
{
    if (argc != 2) // checking correctness of the command line
    {
        printf("<./a.out><inputfile.txt\n");
        exit(1);
    }
    char filename[100];
    sscanf(argv[1], "%s", filename);

    FILE *fptr = fopen(filename, "r"); // opening the file
    if (fptr == NULL)                  // cheking for file error
    {
        printf("Error! opening file");
        exit(1);
    }
    char line[1000];
    fgets(line, 1000, fptr); // taking the input from file
    printf("%s", line);

    int level_order[1000]; // array to store the level order of the tree
    for (int i = 0; i < 1000; i++)
        level_order[i] = -2;                                  // initialisaion
    get_array_from_string(line, level_order); // converting the array of char to array of int and also getting back the length of the array

    for(int i=0;i<1000;i++)
        printf("%d ", level_order[i]);

    return 0;
}

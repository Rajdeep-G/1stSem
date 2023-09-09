#include <stdio.h>
#include <stdlib.h>

// ./a.out c input.txt -> input on command line

struct Presentation
{
    int start_time;
    int end_time;
    int popularity_score;
    char presentation_name[100];
};

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: <program name> <algorithm (a/b)> <input_file>");
        return 0;
    }

    char choice_of_algorithm = argv[1][0];
    char *input_file = argv[2];

    FILE *file = fopen(input_file, "r");

    if (file == NULL)
    {
        printf("Error: Cannot open input file.");
        return 0;
    }

    int no_of_presentations;
    fscanf(file, "%d", &no_of_presentations);  // reading an integer
    printf("%d\n", no_of_presentations);

    struct Presentation *presentations = (struct Presentation *)malloc(no_of_presentations * sizeof(struct Presentation));

    // int *x=(int *)malloc(5*sizeof(int));

    for (int i = 0; i < no_of_presentations; i++)
    {
        fscanf(file, "%d %d %d", &presentations[i].start_time, &presentations[i].end_time, &presentations[i].popularity_score); // rading three nos from the line
        fgets(presentations[i].presentation_name, 100, file); // reading string from a line
    }


    fclose(file);

    return 0;
}
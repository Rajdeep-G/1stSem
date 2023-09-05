#include <stdio.h>
#include <stdlib.h>

struct Presentation
{
    int start_time;
    int end_time;
    int popularity_score;
    char presentation_name[50];
};

void GREEDY_START(struct Presentation *presentations, int no_of_presentations)
{

    for (int i = 0; i < no_of_presentations; i++)
    {
        for (int j = 0; j < no_of_presentations - i - 1; j++)
        {
            if (presentations[j].start_time > presentations[j + 1].start_time)
            {
                struct Presentation temp = presentations[j];
                presentations[j] = presentations[j + 1];
                presentations[j + 1] = temp;
            }
        }
    }
    // for (int i = 0; i < no_of_presentations; i++)
    //     printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    FILE *file = fopen("PartA_output.txt", "w");

    // printf("\nOUTPUT.......\n");
    // printf("\n%d %d %d %s", presentations[0].start_time, presentations[0].end_time, presentations[0].popularity_score, presentations[0].presentation_name);
    fprintf(file, "%d %d %d %s", presentations[0].start_time, presentations[0].end_time, presentations[0].popularity_score, presentations[0].presentation_name);

    int current_endtime = presentations[0].end_time;

    for (int i = 1; i < no_of_presentations; i++)
    {
        if (presentations[i].start_time >= current_endtime)
        {
            // printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            current_endtime = presentations[i].end_time;
        }
    }
}

void GREEDY_POPULARITY(struct Presentation *presentations, int no_of_presentations)
{
    for (int i = 0; i < no_of_presentations; i++)
    {
        for (int j = 0; j < no_of_presentations - i - 1; j++)
        {
            if (presentations[j].popularity_score < presentations[j + 1].popularity_score)
            {
                struct Presentation temp = presentations[j];
                presentations[j] = presentations[j + 1];
                presentations[j + 1] = temp;
            }
        }
    }
    // for (int i = 0; i < no_of_presentations; i++)
    //     printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    FILE *file = fopen("PartB_output.txt", "w");
    // printf("\n......\n");
    // printf("\n%d %d %d %s", presentations[0].start_time, presentations[0].end_time, presentations[0].popularity_score, presentations[0].presentation_name);
    fprintf(file, "%d %d %d %s", presentations[0].start_time, presentations[0].end_time, presentations[0].popularity_score, presentations[0].presentation_name);

    int current_endtime = presentations[0].end_time;
    int current_starttime = presentations[0].start_time;

    for (int i = 1; i < no_of_presentations; i++)
    {
        if (presentations[i].end_time <= current_starttime)
        {
            // printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            current_starttime = presentations[i].start_time;
        }
        else if (presentations[i].start_time >= current_endtime)
        {
            // printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            current_endtime = presentations[i].end_time;
        }
    }
}

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
    fscanf(file, "%d", &no_of_presentations);
    printf("%d\n", no_of_presentations);

    struct Presentation *presentations = (struct Presentation *)malloc(no_of_presentations * sizeof(struct Presentation));

    for (int i = 0; i < no_of_presentations; i++)
    {
        fscanf(file, "%d %d %d", &presentations[i].start_time, &presentations[i].end_time, &presentations[i].popularity_score);
        fgets(presentations[i].presentation_name, 100, file);
    }

    // for (int i = 0; i < no_of_presentations; i++)
    //     printf("%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    fclose(file);

    switch (choice_of_algorithm)
    {

    case 'a':
        printf("CALL GREEDY START");
        GREEDY_START(presentations, no_of_presentations);

        break;
    case 'b':
        printf("CALL GREEDY POPULARITY");
        GREEDY_POPULARITY(presentations, no_of_presentations);
        break;
    default:
        printf("Error: Invalid algorithm option.");
    }

    free(presentations);
    return 0;
}
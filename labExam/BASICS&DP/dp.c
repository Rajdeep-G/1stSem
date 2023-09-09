#include <stdio.h>
#include <stdlib.h>

struct Presentation
{
    int start_time;
    int end_time;
    int popularity_score;
    char presentation_name[50];
};

void sorting_Bubble(struct Presentation *presentations, int no_of_presentations, int asc_or_desc, int choice_of_attribute)
{
    // asc_or_desc = 1 for ascending order, -1 for descending order
    // choice of attribute : 1 -> for start time 2-> for end time 3-> for popularity score
    if (asc_or_desc == 1)
    {
        if (choice_of_attribute == 1)
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
        }

        else if (choice_of_attribute == 2)
        {
            for (int i = 0; i < no_of_presentations; i++)
            {
                for (int j = 0; j < no_of_presentations - i - 1; j++)
                {
                    if (presentations[j].end_time > presentations[j + 1].end_time)
                    {
                        struct Presentation temp = presentations[j];
                        presentations[j] = presentations[j + 1];
                        presentations[j + 1] = temp;
                    }
                }
            }
        }
    }
    else if (asc_or_desc == -1)
    {
        if (choice_of_attribute == 2)
        {
            for (int i = 0; i < no_of_presentations; i++)
            {
                for (int j = 0; j < no_of_presentations - i - 1; j++)
                {
                    if (presentations[j].end_time < presentations[j + 1].end_time)
                    {
                        struct Presentation temp = presentations[j];
                        presentations[j] = presentations[j + 1];
                        presentations[j + 1] = temp;
                    }
                }
            }
        }

        else if (choice_of_attribute == 3)
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
        }
    }
}

int m_max(int a, int b)
{
    if (a >= b)
        return a;

    else
        return b;
}

void GREEDY_START(struct Presentation *presentations, int no_of_presentations)
{

    sorting_Bubble(presentations, no_of_presentations, 1, 1);
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
    sorting_Bubble(presentations, no_of_presentations, -1, 3);
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

int get_Next_Presentation(struct Presentation *presentations, int index, int no_of_presentations)
{
    for (int i = index - 1; i >= 0; i--)
    {
        if (presentations[i].end_time <= presentations[index].start_time)
            return i;
    }
    return -1;
}

int max_score(struct Presentation *presentations, int index, int no_of_presentations, int *dp, int *selected_indices)
{
    dp[0] = presentations[0].popularity_score;
    int track[no_of_presentations];
    track[0] = 0;
    for (int i = 1; i < no_of_presentations; i++)
    {
        int including_score = presentations[i].popularity_score;

        if (get_Next_Presentation(presentations, i, no_of_presentations) != -1)
            including_score += dp[get_Next_Presentation(presentations, i, no_of_presentations)];

        int excluding_score = dp[i - 1];

        // this part is for keeping a track actually
        if (including_score > excluding_score)
            track[i] = get_Next_Presentation(presentations, i, no_of_presentations);
        else
            track[i] = i - 1;

        // taking the max value
        dp[i] = m_max(including_score, excluding_score);
    }
    // print the track array
    
    // this part is basically backtracking the path thorugh which we got the highest value and hence marking in the selected indices array , which eventually helps to print the selected presentatuions

    int i = no_of_presentations - 1;
    while (i >= 0)
    {
        int temp = i;
        if (track[i] == i - 1)
        {
            selected_indices[i] = 0;
            i--;
        }
        else
        {
            selected_indices[i] = 1;
            i = track[i];
        }
        if (temp == i)
            break;
    }

    // final answer is being returned
    return dp[no_of_presentations - 1];
}

void DP_IMPLEMENTATION(struct Presentation *presentations, int no_of_presentations)
{
    sorting_Bubble(presentations, no_of_presentations, 1, 2);

    int selected_indices2[no_of_presentations];
    for (int i = 0; i < no_of_presentations; i++)
        selected_indices2[i] = -1;

    int dp[no_of_presentations];
    int value = max_score(presentations, 0, no_of_presentations, dp, selected_indices2);

    FILE *file = fopen("PartC_output.txt", "w");

    fprintf(file, "Maximum Total Popularity: %d\n", value);
    // printf("Maximum Total Popularity: %d\n", value);
    // printf("\nSelected Presentations: \n");
    fprintf(file, "Selected Presentations: \n");
    printf("\n");

    for (int i = 0; i < no_of_presentations; i++)
    {
        if (selected_indices2[i] == 1)
        {
            // printf("%d %d %d %s\n", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
            fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
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
    case 'c':
        printf("CALL DP IMPLEMENTATION");
        DP_IMPLEMENTATION(presentations, no_of_presentations);
        break;
    default:
        printf("Error: Invalid algorithm option.");
    }

    free(presentations);
    return 0;
}
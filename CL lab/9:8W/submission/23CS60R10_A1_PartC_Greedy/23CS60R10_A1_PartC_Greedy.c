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

void greedy_app_k(struct Presentation *presentations, int no_of_presentations, int no_of_divisions)
{
    int k = no_of_divisions;
    int bestGreedySum = 0;

    sorting_Bubble(presentations, no_of_presentations, 1, 2);

    int timeline_start = presentations[0].start_time;
    int timeline_end = presentations[no_of_presentations - 1].end_time;

    FILE *file = fopen("Output_greedy.txt", "w");

    int max_score_index;
    int i = 0;

    int part_start = timeline_start;
    int part_end = timeline_start + (timeline_end - timeline_start) / k;

    while (k != 1)
    {

        max_score_index = -1;
        for (; i < no_of_presentations; i++)
        {
            if (part_end < presentations[i].end_time)
                break;
            if (presentations[i].start_time >= part_start)
            {
                if ((max_score_index == -1 || presentations[max_score_index].popularity_score < presentations[i].popularity_score))
                {
                    max_score_index = i;
                }
            }
        }
        k--;
        if (max_score_index != -1)
        {
            bestGreedySum += presentations[max_score_index].popularity_score;
            part_start = presentations[max_score_index].end_time;
            part_end = part_start + (timeline_end - part_start) / k;
            // printf("\n%d %d %d %s", presentations[max_score_index].start_time, presentations[max_score_index].end_time, presentations[max_score_index].popularity_score, presentations[max_score_index].presentation_name);
            fprintf(file, "%d %d %d %s", presentations[max_score_index].start_time, presentations[max_score_index].end_time, presentations[max_score_index].popularity_score, presentations[max_score_index].presentation_name);
        }
    }
    // printf("\n%d", bestGreedySum);
    while (i < no_of_presentations && presentations[i].start_time < presentations[max_score_index].end_time)
    {
        i++;
    }
    // same as void_greddy_POPULARITY_SCORE
    if (k == 1 && i < no_of_presentations)
    {
        // printf("....");
        // -1 because sorting by des order
        sorting_Bubble(presentations, no_of_presentations, -1, 3);

        // for(int k=i;k<no_of_presentations;k++){
        //     printf("\n%d %d %d %s", presentations[k].start_time, presentations[k].end_time, presentations[k].popularity_score, presentations[k].presentation_name);
        // }
        // printf("\n......\n");
        // printf("\n%d %d %d %s", presentations[0].start_time, presentations[0].end_time, presentations[0].popularity_score, presentations[0].presentation_name);
        fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

        int current_starttime = presentations[i].start_time;
        int current_endtime = presentations[i].end_time;
        bestGreedySum += presentations[i].popularity_score;

        while (i < no_of_presentations)
        {

            if (presentations[i].end_time <= current_starttime)
            {
                // printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
                fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
                current_starttime = presentations[i].start_time;
                bestGreedySum += presentations[i].popularity_score;
            }
            else if (presentations[i].start_time >= current_endtime)
            {
                // printf("\n%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
                fprintf(file, "%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
                current_endtime = presentations[i].end_time;
                bestGreedySum += presentations[i].popularity_score;
            }
        i++;
        }
        
    }

    // printf("\nMAXIMUM TOTAL POPULARITY SCOREdasdas- %d", bestGreedySum);
    fprintf(file, "\nMAXIMUM TOTAL POPULARITY SCORE- %d", bestGreedySum);
    fclose(file);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: <program name> <input_file>");
        return 0;
    }

    // char choice_of_algorithm = argv[1][0];
    char *input_file = argv[1];

    FILE *file = fopen(input_file, "r");

    if (file == NULL)
    {
        printf("Error: Cannot open input file.");
        return 0;
    }

    int no_of_presentations;
    int no_of_divisions;
    fscanf(file, "%d", &no_of_presentations);
    fscanf(file, "%d", &no_of_divisions);
    // printf("%d\n", no_of_presentations);
    // printf("%d\n", no_of_divisions);

    struct Presentation *presentations = (struct Presentation *)malloc(no_of_presentations * sizeof(struct Presentation));

    for (int i = 0; i < no_of_presentations; i++)
    {
        fscanf(file, "%d %d %d", &presentations[i].start_time, &presentations[i].end_time, &presentations[i].popularity_score);
        fgets(presentations[i].presentation_name, 100, file);
    }

    // for (int i = 0; i < no_of_presentations; i++)
    //     printf("%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    fclose(file);
    // printf("\n%d", no_of_presentations);
    // printf("\n%d", no_of_divisions);
    greedy_app_k(presentations, no_of_presentations, no_of_divisions);

    free(presentations);
    return 0;
}
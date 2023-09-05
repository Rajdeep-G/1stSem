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
    // return presentations;
}

int m_max(int a, int b)
{
    if (a >= b)
        return a;

    else
        return b;
}

int get_Next_Presentation(struct Presentation *presentations, int index, int no_of_presentations)
{
    int next_index = index + 1;
    int current_end_time = presentations[index].end_time;

    while (next_index < no_of_presentations && presentations[next_index].start_time < current_end_time)
        next_index++;

    return next_index;
}

int max_popularity_score(struct Presentation *presentations, int index, int no_of_presentations, int *selected_indices, int memo[][2])
{
    //memo[idx][0] holds the maximum total popularity memo[idx][1] holds a flag indicating 
    // whether the presentation at index idx should be included  or excluded in the optimal solution for the subproblem.
    if (index >= no_of_presentations)
        return 0;

    //returning the value if already computed.
    if (memo[index][0] != -1)
    {
        if (memo[index][1] == 1) // if the value is included, we mark that in the selected matrix for final printing
        {
            selected_indices[index] = 1;
        }
        else
        {
            selected_indices[index] = -1; 
        }
        return memo[index][0]; // returning the already computed value
    }

    int next = get_Next_Presentation(presentations, index, no_of_presentations);
    int include_score = presentations[index].popularity_score + max_popularity_score(presentations, next, no_of_presentations, selected_indices, memo);
    int exclude_score = max_popularity_score(presentations, next, no_of_presentations, selected_indices, memo);

    if (include_score >= exclude_score)
    {
        selected_indices[index] = 1;
        // memo[index][selected_indices[index]] = include_score;
        memo[index][1]=include_score;
    }
    else
    {
        selected_indices[index] = -1;
        // memo[index][selected_indices[index]] = exclude_score;
        memo[index][1]=exclude_score;
    }

    // return memo[index][selected_indices[index]];
    return memo[index][1];
}

void dp_implementation(struct Presentation *presentations, int no_of_presentations)
{
    sorting_Bubble(presentations, no_of_presentations, 1, 2);

    int selected_indices[no_of_presentations];
    for (int i = 0; i < no_of_presentations; i++)
        selected_indices[i] = -1;

    int memo[no_of_presentations][2];
    for (int i = 0; i < no_of_presentations; i++)
    {
        memo[i][0] = -1;
        memo[i][1] = -1;
    }
    int value = max_popularity_score(presentations, 0, no_of_presentations, selected_indices, memo);

    printf("Maximum Total Popularity: %d\n", value);
    printf("\nSelected Presentations: \n");

    for (int i = 0; i < no_of_presentations; i++)
    {
        if (selected_indices[i] == 1)
        {
            printf("%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);
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
    // printf("%d\n", no_of_presentations);

    struct Presentation *presentations = (struct Presentation *)malloc(no_of_presentations * sizeof(struct Presentation));

    for (int i = 0; i < no_of_presentations; i++)
    {
        fscanf(file, "%d %d %d", &presentations[i].start_time, &presentations[i].end_time, &presentations[i].popularity_score);
        fgets(presentations[i].presentation_name, 100, file);
    }

    // for (int i = 0; i < no_of_presentations; i++)
    //     printf("%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    fclose(file);

    dp_implementation(presentations, no_of_presentations);

    free(presentations);
    return 0;
}
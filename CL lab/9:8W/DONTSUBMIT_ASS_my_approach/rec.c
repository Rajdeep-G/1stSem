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

int get_Next_Presentation(struct Presentation *presentations, int idx, int no_of_presentations)
{
    int i = idx + 1;
    while (i < no_of_presentations && presentations[i].start_time < presentations[idx].end_time)
        i++;
    printf("\n%d", i);
    return i;
}

int max_score(struct Presentation *presentations, int idx, int no_of_presentations, int *selected_indices)
{
    if (idx >= no_of_presentations)
        return 0;
    else
    {
        int next = get_Next_Presentation(presentations, idx, no_of_presentations);
        int include = presentations[idx].popularity_score + max_score(presentations, next, no_of_presentations, selected_indices);
        int exclude = max_score(presentations, next, no_of_presentations, selected_indices);
        if (include >= exclude)
        {
            selected_indices[idx] = 1;
            return include;
        }
        else
        {
            selected_indices[idx] = -1;
            return exclude;
        }
    }
}

void recursive_implementaion(struct Presentation *presentations, int no_of_presentations)
{
    sorting_Bubble(presentations, no_of_presentations, 1, 2);

    for (int i = 0; i < no_of_presentations; i++)
        printf("%d %d %d %s", presentations[i].start_time, presentations[i].end_time, presentations[i].popularity_score, presentations[i].presentation_name);

    int selected_indices[no_of_presentations];
    
    for (int i = 0; i < no_of_presentations; i++)
        selected_indices[i] = -1;

    printf("\n");
    int value = max_score(presentations, 0, no_of_presentations, selected_indices);
    printf("%d\n", value);

    for (int i = 0; i < no_of_presentations; i++)
        printf("%d", selected_indices[i]);

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

    recursive_implementaion(presentations, no_of_presentations);

    free(presentations);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 100

// change the direction to int datatype and character to 1 , 2 and 3 (1- diagnoal 2 -left 3-right )
// and make 3 cases one for left priority , one for right priority and one for random priority
struct LCSCell
{
    int length;
    int direction;
};
int ran_gen(int n)
{

    // printf("%d", (rand() % 2) + 2);
    return (rand() % 2) + 2;
}
void findLCS(char *str1, char *str2, struct LCSCell lcsTable[][MAX_LENGTH], int choice_user)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                lcsTable[i][j].length = lcsTable[i - 1][j - 1].length + 1;
                lcsTable[i][j].direction = '1';
            }
            else
            {

                if (lcsTable[i - 1][j].length == lcsTable[i][j - 1].length)
                {
                    switch (choice_user)
                    {
                    case 2:
                    {
                        lcsTable[i][j].length = lcsTable[i][j - 1].length;
                        lcsTable[i][j].direction = '2';
                        break;
                    }

                    case 3:
                    {
                        lcsTable[i][j].length = lcsTable[i - 1][j].length;
                        lcsTable[i][j].direction = '3';
                        break;
                    }
                    case 1:
                    {
                        printf("success");
                        int k = ran_gen(choice_user);
                        if (k == 2)
                        {
                            lcsTable[i][j].length = lcsTable[i][j - 1].length;
                            lcsTable[i][j].direction = '2';
                        }
                        else
                        {
                            lcsTable[i][j].length = lcsTable[i - 1][j].length;
                            lcsTable[i][j].direction = '3';
                        }
                        break;
                    }
                    default:
                        printf("wrong choice");
                        
                    }
                    
                }
                else if (lcsTable[i - 1][j].length > lcsTable[i][j - 1].length)
                {
                    lcsTable[i][j].length = lcsTable[i - 1][j].length;
                    lcsTable[i][j].direction = '3';
                }
                else
                {
                    lcsTable[i][j].length = lcsTable[i][j - 1].length;
                    lcsTable[i][j].direction = '2';
                }
            }
        }
    }
    printf("\n");
    for (int i = 0; i <= len1; i++)
    {
        for (int j = 0; j <= len2; j++)
        {
            printf("%d ", lcsTable[i][j].length);
        }
        printf("\n");
    }

    for (int i = 0; i <= len1; i++)
    {
        for (int j = 0; j <= len2; j++)
        {
            printf("%c ", lcsTable[i][j].direction);
        }
        printf("\n");
    }
}

void printLCS(struct LCSCell lcsTable[][MAX_LENGTH], char *str1, int i, int j)
{
    if (i == 0 || j == 0)
    {
        return;
    }
    if (lcsTable[i][j].direction == '1')
    {
        printLCS(lcsTable, str1, i - 1, j - 1);
        printf("%c", str1[i - 1]);
    }
    else if (lcsTable[i][j].direction == '3')
    {
        printLCS(lcsTable, str1, i - 1, j);
    }
    else if (lcsTable[i][j].direction == '2')
    {
        printLCS(lcsTable, str1, i, j - 1);
    }
}

int main()
{
    char str1[MAX_LENGTH];
    char str2[MAX_LENGTH];

    printf("Enter the first string: ");
    scanf("%s", str1);

    printf("Enter the second string: ");
    scanf("%s", str2);
    // char str1[] = "XYZABCA";
    // char str2[] = "BCAXYZZ";
    int choice_of_direction;
    printf("\nPress 1 for random with 50-50 probablity and 2 for left and 3 for right");
    scanf("%d", &choice_of_direction);

    struct LCSCell lcsTable[MAX_LENGTH][MAX_LENGTH] = {0};

    findLCS(str1, str2, lcsTable, choice_of_direction);

    printf("Longest Common Subsequence: ");
    printLCS(lcsTable, str1, strlen(str1), strlen(str2));
    printf("\n");

    return 0;
}

#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100
// change the direction to int datatype 
struct LCSCell {
    int length;
    char direction;
};

void findLCS(char *str1, char *str2, struct LCSCell lcsTable[][MAX_LENGTH]) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                lcsTable[i][j].length = lcsTable[i - 1][j - 1].length + 1;
                lcsTable[i][j].direction = '\\';
            } else {
                if (lcsTable[i - 1][j].length >= lcsTable[i][j - 1].length) {
                    lcsTable[i][j].length = lcsTable[i - 1][j].length;
                    lcsTable[i][j].direction = '|';
                } else {
                    lcsTable[i][j].length = lcsTable[i][j - 1].length;
                    lcsTable[i][j].direction = '-';
                }
            }
        }
    }
     for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            printf("%d ", lcsTable[i][j].length);
        }
        printf("\n");
    }

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            printf("%c ", lcsTable[i][j].direction);
        }
        printf("\n");
    }
}

void printLCS(struct LCSCell lcsTable[][MAX_LENGTH], char *str1, int i, int j) {
    if (i == 0 || j == 0) {
        return;
    }
    if (lcsTable[i][j].direction == '\\') {
        printLCS(lcsTable, str1, i - 1, j - 1);
        printf("%c", str1[i - 1]);
    } else if (lcsTable[i][j].direction == '|') {
        printLCS(lcsTable, str1, i - 1, j);
    } else {
        printLCS(lcsTable, str1, i, j - 1);
    }
    
}

int main() {
    char str1[]="xyzabca";
    char str2[]="bcaxyzz";

    struct LCSCell lcsTable[MAX_LENGTH][MAX_LENGTH] = {0};

    findLCS(str1, str2, lcsTable);

    printf("Longest Common Subsequence: ");
    printLCS(lcsTable, str1, strlen(str1), strlen(str2));
    printf("\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int compareJobs(const int jobA[2], const int jobB[2]) {
    if (jobA[1] == jobB[1]) {
        return jobA[0] - jobB[0];
    } else {
        return jobA[1] - jobB[1];
    }
}

void swap(int jobA[2], int jobB[2]) {
    int temp[2];
    temp[0] = jobA[0];
    temp[1] = jobA[1];
    jobA[0] = jobB[0];
    jobA[1] = jobB[1];
    jobB[0] = temp[0];
    jobB[1] = temp[1];
}

int main() {
    const char* inputFileName = "resource.txt";
    const char* outputFileName = "Plan.txt";

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    int numJobs = 0;
    int jobs[10][2];

    while (!feof(inputFile) && numJobs < 10) {
        if (fscanf(inputFile, "%d %d", &jobs[numJobs][0], &jobs[numJobs][1]) == 2) {
            numJobs++;
        }
    }

    fclose(inputFile);

    // Bubble Sort
    for (int i = 0; i < numJobs - 1; i++) {
        for (int j = 0; j < numJobs - i - 1; j++) {
            if (compareJobs(jobs[j], jobs[j + 1]) > 0) {
                swap(jobs[j], jobs[j + 1]);
            }
        }
    }

    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Error creating file");
        return 1;
    }

    for (int i = 0; i < numJobs; i++) {
        printf("[%d %d] -> ", jobs[i][0], jobs[i][1]);
        fprintf(outputFile, "[%d %d]\n", jobs[i][0], jobs[i][1]);
    }
    
    printf("END\n");
    fclose(outputFile);

    return 0;
}

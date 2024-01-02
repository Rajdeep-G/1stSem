#include <stdio.h>
#include <stdlib.h>

// Structure to hold job information
struct Job {
    int resourceId;
    int executionTime;
};

// Compare function for qsort
int compareJobs(const void* a, const void* b) {
    struct Job* jobA = (struct Job*)a;
    struct Job* jobB = (struct Job*)b;

    // Sort by execution time, and if equal, sort by resource id
    if (jobA->executionTime == jobB->executionTime) {
        return jobA->resourceId - jobB->resourceId;
    } else {
        return jobA->executionTime - jobB->executionTime;
    }
}

int main() {
    // Maximum number of jobs
    const int maxJobs = 10;

    // Read jobs from file
    FILE* inputFile = fopen("resource.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct Job jobs[maxJobs];
    int numJobs = 0;

    while (fscanf(inputFile, "%d %d", &jobs[numJobs].resourceId, &jobs[numJobs].executionTime) == 2) {
        numJobs++;
        if (numJobs >= maxJobs) {
            break; // We can't exceed maxJobs
        }
    }

    fclose(inputFile);

    // Sort the jobs array
    qsort(jobs, numJobs, sizeof(struct Job), compareJobs);

    // Print sorted jobs to terminal and file
    FILE* outputFile = fopen("Plan.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating file");
        return 1;
    }

    for (int i = 0; i < numJobs; i++) {
        printf("[%d %d] ", jobs[i].resourceId, jobs[i].executionTime);
        fprintf(outputFile, "[%d %d]\n", jobs[i].resourceId, jobs[i].executionTime);
    }

    fclose(outputFile);

    return 0;
}

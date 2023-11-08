#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

void *thread_handler(void *data)
{
    int p = *((int *)data);
    // printf("%d\n", p);
    FILE *fptr = fopen("ls.txt", "r"); // file having all html file nanmes
    if (fptr == NULL)
    {
        return 0;
    }
    char str[100];
    int i = 0;
    while (1)
    {
        fscanf(fptr, "%s", str);
        if (i == p)
        {
            // printf("%s", str);
            break; // reqd file for that particular thread
        }
        i++;
    }
    fclose(fptr);
    pid_t child_pid;
    child_pid = fork(); // Create a child process
    if (child_pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    if (child_pid == 0)
    {

        char fname[50];
        strcpy(fname, "parsed_data.log");
        char *parameter[100];
        parameter[0] = "/usr/bin/python3";
        parameter[1] = "parser.py";
        parameter[2] = str;
        parameter[3] = "parsed_data.log";
        execvp(parameter[0], parameter);
        // char *args[] = {param, NULL};
        // execvp(args[0], args);
        // execvp(param, NULL);
        // char *args[] = {"python3 p1.py amazon-com-inc.html parsed_data.log", NULL};
        // execvp(args[0], args);

        // execlp("usr/bin/python3", "python3", "p1.py", NULL);

        // system("python3 p1.py amazon-com-inc.html parsed_data.log");
        // system(param);
    }
    else
    {
        int status;

        waitpid(child_pid, &status, 0);
    }
}

int count_no_zip() // counting no of fhtml files using system call
{
    system("ls files > ls.txt");
    FILE *fptr = fopen("ls.txt", "r");
    char str[100];
    int count = 0;
    while (fgets(str, sizeof(str), fptr))
        count++;

    // printf("count is - %d\n", count);
    return count;
}

void findmax(int count_files)
{
    FILE *fptr = fopen("parsed_data.log", "r");
    int i = 0;
    char str[100], maxn[100];
    double maxp = 0, price = 0;

    while (fgets(str, sizeof(str), fptr))
    {
        fscanf(fptr, "%lf", &price);
        // printf("%lf", price);
        // printf("%s", str);
        if (price >= maxp)
        {
            strcpy(maxn, str);

            maxp = price;
        }
        i++;
    }
    printf("\nmax stock price- %lf", maxp);
    // printf("....\n");
    int k = 0;
    while (fgets(str, sizeof(str), fptr))
    {
        fscanf(fptr, "%lf", &price);

        if (price == maxp)
        {
            printf("%s", str);
        }
        k++;
    }

    // printf("\nNAME of max stock-  %s", maxn);
    fclose(fptr);
}
// Driver code
int main()
{
    int count_files = count_no_zip();
    // int n = count_no_zip();
    // int n = 2;
    FILE *file = fopen("parsed_data.log", "w");
    if (file == NULL)
    {
        perror("Error opening parsed_data.log file");
        exit(1);
    }
    int *serial = (int *)malloc(100 * sizeof(int));
    for (int j = 0; j < 100; j++)
    {
        serial[j] = j + 1;
    }
    pthread_t scrapping_threads[count_files];
    for (int i = 0; i < count_files; i++)
    {
        int error = pthread_create(&(scrapping_threads[i]), NULL, thread_handler, (void *)&serial[i]); // calling thr hread functions
        if (error != 0)
            printf("\nThread can't be created :");
    }

    for (int i = 0; i < count_files; i++)
        pthread_join(scrapping_threads[i], NULL); // joining threads
    fclose(file);
    findmax(count_files); // calculting max profit of cost

    return 0;
}

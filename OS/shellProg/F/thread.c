#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ncurses.h>
#include <pthread.h>
#define MAX_VECTOR_SIZE 1000
struct ThreadArgs targs;
struct ThreadArgs
{
    // char *file1;
    // char *file2;
    int *vector1;
    int *vector2;
    int no_thread;
    int *result;
    int *s_index;
    int *e_index;
    char *command;
};

void *vector_addition(void *k)
{
    // struct ThreadArgs *targs = (struct ThreadArgs *)args;
    int myval = *(int *)k;
    int start = targs.s_index[myval];
    int end = targs.e_index[myval];
    for (int i = start; i < end; i++)
    {
        if (strcmp(targs.command, "addvec") == 0)
            targs.result[i] = targs.vector1[i] + targs.vector2[i];
        else if (strcmp(targs.command, "subvec") == 0)
            targs.result[i] = targs.vector1[i] - targs.vector2[i];
        else if (strcmp(targs.command, "dotprod") == 0)
            targs.result[i] = targs.vector1[i] * targs.vector2[i];
    }
    pthread_exit(NULL);
}

int remove_last_amp(char *user_input)
{
    int bg_process_flag = 0;
    if (user_input[strlen(user_input) - 1] == '&')
    {
        bg_process_flag = 1;
        user_input[strlen(user_input) - 1] = '\0';
        if (strcmp(user_input, "") == 0)
            user_input[strlen(user_input) - 1] = '\0';
    }
    return bg_process_flag;
}

int execute_piped_command(char *command)
{
    char *input[1000];
    int count = 0;
    char *token = strtok(command, "|");
    while (token != NULL)
    {
        input[count++] = token;
        token = strtok(NULL, "|");
    }
    input[count] = NULL;

    // check for the last command
    char *last_command = input[count - 1];

    int bg_process_flag = 0;

    // Create an array to store file descriptors for pipes
    int pipe_fds[count - 1][2];

    for (int i = 0; i < count; i++)
    {
        // Trim leading and trailing spaces from each command
        char *cmd = input[i];
        while (*cmd == ' ')
            cmd++;
        int len = strlen(cmd);
        while (len > 0 && cmd[len - 1] == ' ')
            cmd[--len] = '\0';
        // ...............................................................
        // Tokenize the command into arguments
        char *parameter[1000];
        int no_param = 0;
        token = strtok(cmd, " ");
        while (token != NULL)
        {
            parameter[no_param++] = token;
            token = strtok(NULL, " ");
        }
        parameter[no_param] = NULL;
        // ...............................................................

        int bg_process_flag = 0;
        if (i == count - 1)
            bg_process_flag = remove_last_amp(parameter[no_param - 1]);

        if (i < count - 1)
        {
            // If not the last command, create all pipes
            if (pipe(pipe_fds[i]) == -1)
            {
                perror("Pipe failed");
                exit(1);
            }
        }

        pid_t child_pid;
        child_pid = fork(); // Create a child process
        if (child_pid == -1)
        {
            perror("Fork failed");
            exit(1);
        }
        if (child_pid == 0)
        {
            if (i > 0)
            {
                // Connect stdin to the read end of the previous pipe
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
                close(pipe_fds[i - 1][0]);
            }
            if (i < count - 1)
            {
                // Connect stdout to the write end of the current pipe
                dup2(pipe_fds[i][1], STDOUT_FILENO);
                close(pipe_fds[i][1]);
            }
            // Execute the command
            if (strcmp(parameter[0], "cd") == 0)
            {
                if (chdir(parameter[1]) == -1)
                {
                    perror("chdir failed");
                    exit(1);
                }
            }
            else if (execvp(parameter[0], parameter) == -1)
            {
                perror("execvp failed");
                exit(1);
            }
        }
        else
        {
            // Close the write end of the current pipe (if not the last command)
            if (i < count - 1)
            {
                close(pipe_fds[i][1]);
            }
            // Wait for the child process
            if (bg_process_flag != 1)
            {
                int status;
                waitpid(child_pid, &status, 0);
            }
        }
    }

    // Close all pipe file descriptors
    for (int i = 0; i < count - 1; i++)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    return 0;
}

int execute_command(char *command)
{

    if (strstr(command, "|"))
    {
        // If the command contains a pipe character, execute as piped command
        execute_piped_command(command);
        // printf("Piped commands not supported yet\n");
    }
    else
    {
        // Otherwise, execute as a regular command
        char *input[1000];
        int count = 0;
        char *token = strtok(command, " ");
        while (token != NULL)
        {
            input[count++] = token;
            token = strtok(NULL, " ");
        }
        input[count] = NULL;
        int bg_process_flag = 0;

        if (count > 1)
            bg_process_flag = remove_last_amp(input[count - 1]);

        // check if its a vi command
        if (strcmp(input[0], "cd") == 0)
        {
            if (chdir(input[1]) == -1)
            {
                perror("chdir failed");
                exit(1);
            }
            return 0;
        }
        pid_t child_pid;
        child_pid = fork(); // Create a child process
        if (child_pid == -1)
        {
            perror("Fork failed");
            exit(1);
        }
        if (child_pid == 0)
        {
            if (strcmp(input[0], "vi") == 0)
            {
                execlp("./vi", "vi", input[1], NULL);
            }
            else if (strcmp(input[0], "addvec") == 0 || strcmp(input[0], "subvec") == 0 || strcmp(input[0], "dotprod") == 0)
            {
                if (count < 4)
                {
                    printf("Usage: addvec <file1> <file2> <no_thread>\n");
                    exit(1);
                }
                else
                {
                    char *file1 = input[1];
                    char *file2 = input[2];
                    FILE *fp1 = fopen(file1, "r");
                    FILE *fp2 = fopen(file2, "r");
                    if (fp1 == NULL || fp2 == NULL)
                    {
                        perror("Error opening files");
                        exit(1);
                    }
                    int no_thread = 3;
                    if (count > 3 && input[3][0] == '-')
                        no_thread = atoi(input[3] + 1);
                    targs.no_thread = no_thread;
                    targs.command = command;
                    targs.vector1 = (int *)malloc(MAX_VECTOR_SIZE * sizeof(int));
                    targs.vector2 = (int *)malloc(MAX_VECTOR_SIZE * sizeof(int));
                    printf("No of threads: %d\n", no_thread);
                    targs.result = (int *)malloc(MAX_VECTOR_SIZE * sizeof(int));
                    targs.s_index = (int *)malloc(no_thread * sizeof(int));
                    targs.e_index = (int *)malloc(no_thread * sizeof(int));
                    pthread_t threads[MAX_VECTOR_SIZE];
                    int no_of_numbers = 0;
                    while (fscanf(fp1, "%d", &targs.vector1[no_of_numbers]) == 1 && fscanf(fp2, "%d", &targs.vector2[no_of_numbers]) == 1)
                        no_of_numbers++;
                    int i;
                    int chunk_size = no_of_numbers / no_thread;
                    int remainder = no_of_numbers % no_thread;
                    printf("Chunk size: %d\n", chunk_size);
                    targs.s_index[0] = 0;
                    targs.e_index[0] = chunk_size + (remainder > 0 ? 1 : 0);
                    for (int k = 1; k < no_thread; k++)
                    {
                        targs.s_index[k] = targs.e_index[k - 1];
                        targs.e_index[k] = targs.s_index[k] + chunk_size + (remainder > k ? 1 : 0);
                    }

                    for (int k = 0; k < no_thread; k++)
                        printf("%d %d\n", targs.s_index[k], targs.e_index[k]);
                    int *myval = (int *)malloc(no_thread * sizeof(int));
                    for (i = 0; i < no_thread; i++)
                    {
                        myval[i] = i;
                        if (pthread_create(&threads[i], NULL, vector_addition, (void *)&myval[i]) != 0)
                        {
                            perror("Error creating thread");
                            exit(1);
                        }
                    }

                    for (i = 0; i < no_thread; i++)
                        pthread_join(threads[i], NULL);

                    printf("Vector Operation result\n");
                    if (strcmp(input[0], "dotprod") == 0)
                    {
                        int sum = 0;
                        for (int j = 0; j < no_of_numbers; j++)
                            sum += targs.result[j];
                        printf("Dot product result -> %d\n", sum);
                    }
                    else
                    {
                        if (strcmp(input[0], "addvec") == 0)
                            printf("Vector Addition result -> ");
                        else
                            printf("Vector Subtraction result -> ");
                        for (int j = 0; j < no_of_numbers; j++)
                            printf("%d ", targs.result[j]);
                        printf("\n");
                    }
                }
            }

            else if (execvp(input[0], input) == -1)
            {
                perror("execvp failed");
                exit(1);
            }
        }
        else
        {
            if (bg_process_flag != 1)
            {
                int status;
                waitpid(child_pid, &status, 0);
            }
        }
    }
    return 0;
}

void modification_for_multi_line(char *user_input)
{
    while (strlen(user_input) > 0 && user_input[strlen(user_input) - 1] == '\\')
    {
        user_input[strlen(user_input) - 1] = '\0';
        char *next_line = readline("> ");
        if (next_line != NULL)
        {
            user_input = realloc(user_input, strlen(user_input) + strlen(next_line) + 1);
            strcat(user_input, next_line);
            free(next_line);
        }
        else
            break;
    }
}

int main()
{
    char *user_input = NULL;
    while (1)
    {
        user_input = readline("Myshell> ");
        if (!user_input)
        {
            printf("\n");
            free(user_input);
            return 0;
        }

        if (strlen(user_input) > 0)
        {

            add_history(user_input); // Add the input to the command history.
            if (strcmp(user_input, "exit") == 0)
            {
                free(user_input);
                return 0;
            }
            else if (strcmp(user_input, "help") == 0)
            {
                printf("Available commands:\n");
                printf("1. pwd - shows the present working directory\n");
                printf("2. cd <directory_name> - changes the present working directory\n");
                printf("3. mkdir <directory_name> - creates a new directory\n");
                printf("4. ls <flag> - shows the contents of a directory with optional flags\n");
                printf("5. exit - exits the shell\n");
                printf("6. help - prints this list of commands\n");
            }
            else
            {
                modification_for_multi_line(user_input);
                execute_command(user_input);
            }
        }

        free(user_input);
    }

    return 0;
}

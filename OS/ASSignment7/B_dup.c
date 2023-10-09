#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

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

    // remove all leading and trailing spaces from the input
    for (int i = 0; i < count; i++)
    {
        int j = 0;
        while (input[i][j] == ' ')
            j++;
        int k = strlen(input[i]) - 1;
        while (input[i][k] == ' ')
            k--;
        for (int l = j; l <= k; l++)
            input[i][l - j] = input[i][l];
        input[i][k - j + 1] = '\0';
    }

    for (int i = 0; i < count; i++)
    {
        printf("%s\n", input[i]);
    }
    printf("%d", count);
// .........
    int prev_pipe_read = -1;
    // int fd[2];
    // pipe(fd);

    for (int i = 0; i < count; i++)
    {
        int fd[2];
        pipe(fd);

        pid_t child_pid;
        child_pid = fork(); // Create a child process

        if (child_pid == -1)
        {
            perror("Fork failed");
            exit(1);
        }

        if (child_pid == 0)
        {
            // Child process

            // Close the read end of the previous pipe (if any)
            if (prev_pipe_read != -1)
            {
                close(prev_pipe_read);
            }

            // Redirect the standard input if not the first command
            if (i > 0)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            // Redirect the standard output to the write end of the pipe
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
 
            char *command = input[i];
            char *args[100];
            int arg_count = 0;
            char *arg_token = strtok(command, " ");
            while (arg_token != NULL)
            {
                args[arg_count++] = arg_token;
                arg_token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            if (execvp(args[0], args) == -1)
            {
                perror("execvp failed");
                exit(1);
            }
        }
        else
        {
            // Parent process
            // Close the write end of the pipe
            close(fd[1]);
            // Close the read end of the previous pipe (if any)
            if (prev_pipe_read != -1)
                close(prev_pipe_read);

            // Store the read end of the current pipe as the previous for the next iteration
            prev_pipe_read = fd[0];

            waitpid(child_pid, NULL, 0);
        }
    }

    // Close the last read end of the pipe
    if (prev_pipe_read != -1)
    {
        close(prev_pipe_read);
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

        if (input[count - 1][strlen(input[count - 1]) - 1] == '&')
        {
            bg_process_flag = 1;
            input[count - 1][strlen(input[count - 1]) - 1] = '\0';
            if (strcmp(input[count - 1], "") == 0)
            {
                count -= 1;
                input[count] = NULL;
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
            if (strcmp(input[0], "cd") == 0)
            {
                if (chdir(input[1]) == -1)
                {
                    perror("chdir failed");
                    exit(1);
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

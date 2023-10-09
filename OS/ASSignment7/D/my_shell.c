#include "vi_editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int execute_command(char *command)
{

    if (strstr(command, "|"))
    {
        // If the command contains a pipe character, execute as piped command
        // execute_piped_command(command);
        printf("Piped commands not supported yet\n");
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

        if (strcmp(input[0], "vi") == 0)
        {
            if (input[1] != NULL)
            {
                pid_t child_pid;
                child_pid = fork();
                if (child_pid == -1)
                {
                    perror("Fork failed");
                    exit(1);
                }
                if (child_pid == 0)
                {
                    // Execute the Vi editor with the specified filename
                    execlp("./vi_editor", "vi_editor", input[1], NULL);
                    perror("execlp failed");
                    exit(1);
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
            else
            {
                printf("Usage: vi <filename>\n");
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

            // if (strncmp(user_input, "vi ", 3) == 0)
            // {
            //     // User wants to open Vi editor with a file, e.g., "vi filename"
            //     char *filename = user_input + 3; // Extract filename
            //     // Launch the Vi editor to edit the specified file
            //     vi_editor(filename);
            // }
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

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






int main() {
    char *user_input = NULL;

    while (1) {
        user_input = readline("Myshell> ");
        if (!user_input) {
            // Handle Ctrl+D (EOF) to exit gracefully.
            printf("\n");
            free(user_input);
            return 0;
        }

        if (strlen(user_input) > 0) {
            // Add the input to the command history.
            add_history(user_input);

            if (strcmp(user_input, "exit") == 0) {
                free(user_input);
                return 0;
            } else if (strcmp(user_input, "help") == 0) {
                printf("Available commands:\n");
                printf("1. pwd - shows the present working directory\n");
                printf("2. cd <directory_name> - changes the present working directory\n");
                printf("3. mkdir <directory_name> - creates a new directory\n");
                printf("4. ls <flag> - shows the contents of a directory with optional flags\n");
                printf("5. exit - exits the shell\n");
                printf("6. help - prints this list of commands\n");
            } else {
                // Check if the last character is a backslash for multi-line input.
                while (strlen(user_input) > 0 && user_input[strlen(user_input) - 1] == '\\') {
                    // Remove the backslash and continue reading the next line.
                    user_input[strlen(user_input) - 1] = '\0';
                    char *next_line = readline("     > ");
                    if (next_line) {
                        // Concatenate the next line.
                        user_input = realloc(user_input, strlen(user_input) + strlen(next_line) + 1);
                        strcat(user_input, next_line);
                        free(next_line);
                    } else {
                        break;  // Handle Ctrl+D (EOF) in continuation lines.
                    }
                }
                execute_command(user_input);
            }
        }

        free(user_input);
    }
    
    return 0;
}




// int main()
// {
//     // char user_input[100];
//     while (1)
//     {
//         char *user_input = readline("Myshell> ");
//         // printf("Myshell> ");
//         // fgets(user_input, 100, stdin);
//         // user_input[strcspn(user_input, "\n")] = '\0';
//         if (!user_input)
//         {
//             return 0;
//         }
//         if (strlen(user_input) > 0)
//         {
//             add_history(user_input);

//             if (strcmp(user_input, "exit") == 0)
//             {
//                 free(user_input);
//                 return 0;
//             }
//             else if (strcmp(user_input, "help") == 0)
//             {
//                 printf("Available commands:\n");
//                 printf("1. pwd - shows the present working directory\n");
//                 printf("2. cd <directory_name> - changes the present working directory\n");
//                 printf("3. mkdir <directory_name> - creates a new directory\n");
//                 printf("4. ls <flag> - shows the contents of a directory with optional flags\n");
//                 printf("5. exit - exits the shell\n");
//                 printf("6. help - prints this list of commands\n");
//             }
//             else
//             {
//                 execute_command(user_input);
//             }
//         }
//         free(user_input);
//     }
//     return 0;
// }

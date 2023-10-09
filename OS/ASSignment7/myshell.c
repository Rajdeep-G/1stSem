#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int execute_command(char *command)
{
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

    // printf("\n%c", input[count - 1][strlen(input[count - 1]) - 1]);
    // printf("\n%d\n", bg);

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
    // printf("\n%d\n", bg);
    pid_t child_pid;
    child_pid = fork(); // Create a child process
    if (child_pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    if (child_pid == 0)
    {
        // printf("%s",input[1]);
        // rintf("Inside CHild\n");
        if (strcmp(input[0], "cd") == 0)
        {
            chdir(input[1]);
        }
        else if (execvp(input[0], input) == -1)
        {
            perror("execlp failed");
            exit(1);
        }
    }
    else
    {
        // printf("Child has executed the code\n");
        if (bg_process_flag != 1)
        {
            int status;
            // wait(&status);
            waitpid(child_pid, &status, 0);
            // if (WIFEXITED(status))
            // printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}

int main()
{
    char user_input[100];
    while (1)
    {
        printf("Myshell> ");
        fgets(user_input, 100, stdin);
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "exit") == 0)
        {
            // printf("ccc");
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
            execute_command(user_input);
    }

    return 0;
}

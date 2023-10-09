// #Compile the hello_world program then compile this program so that it execute whatever the child program[hello_world.txt] wants to achieve[here, creating a output.txt file with some info]

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t child_pid;

    child_pid = fork(); // Create a child process

    if (child_pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0)
    {
        // This code runs in the child process
        printf("Inside CHild\n");
        // Use execlp to execute another C program (e.g., hello_world)
        if (execlp("./hello_world", "hello_world", NULL) == -1)
        {
            perror("execlp failed");
            exit(1);
        }
    }
    else
    {
        // This code runs in the parent process
        printf("Child has executed the code\n");

        // Wait for the child to complete
        int status;
        wait(&status);

        if (WIFEXITED(status))
        {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}

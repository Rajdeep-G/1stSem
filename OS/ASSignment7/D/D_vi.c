#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ncurses.h>

void run_text_editor(const char *filename)
{
    int modified_lines = 0;
    int modified_words = 0;
    int modified_characters = 0;
    initscr();            // Initialize the window
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable arrow keys
    noecho();             // Don't echo user input

    FILE *file = fopen(filename, "r+");
    if (file == NULL)
    {
        endwin();
        perror("Error opening file");
        return;
    }

    // Read file contents into memory
    char **lines = NULL;
    int num_lines = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file))
    {
        lines = realloc(lines, (num_lines + 1) * sizeof(char *));
        lines[num_lines] = strdup(buffer);
        num_lines++;
    }

    // Initialize cursor position
    int cursor_x = 0, cursor_y = 0;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Get the screen dimensions

    int ch;
    while (1)
    {
        // Display the contents of the file
        clear();
        for (int i = 0; i < max_y && cursor_y + i < num_lines; i++)
        {
            mvprintw(i, 0, lines[cursor_y + i]);
        }
        // move(cursor_x, cursor_y);
        move(cursor_y, cursor_x);

        refresh();
        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            if (cursor_x > 0)
                cursor_x--;
            break;
        case KEY_DOWN:
            if (cursor_x < max_x - 1)
                cursor_x++;
            break;
        case KEY_LEFT:
            if (cursor_y > 0)
                cursor_y--;
            break;
        case KEY_RIGHT:
            if (cursor_y < max_y - 1)
                cursor_y++;
            break;
            // case KEY_DC: // Delete key
            // case KEY_BACKSPACE:
        case ' ': // Space bar key
            if (cursor_x > 0 && cursor_x <= strlen(lines[cursor_y]))
            {
                cursor_x--; // Move the cursor left
                for (int i = cursor_x; i < strlen(lines[cursor_y]); i++)
                {
                    lines[cursor_y][i] = lines[cursor_y][i + 1];
                }
                modified_characters++;
            }

            break;
        case 19: // Ctrl+S
            printf("Saving file...\n");
            fseek(file, 0, SEEK_SET);
            for (int i = 0; i < num_lines; i++)
            {
                fputs(lines[i], file);
            }

            mvprintw(max_y - 1, 0, "File saved");
            refresh();
            break;
        case 27: // Escape key
        case 24: // Ctrl+X

            endwin();
            printf("Number of lines modified: %d\n", modified_lines);
            printf("Number of words modified: %d\n", modified_words);
            printf("Number of characters modified: %d\n", modified_characters);
            return;
        default:
            if (cursor_x < strlen(lines[cursor_y]))
            {
                memmove(&lines[cursor_y][cursor_x + 1], &lines[cursor_y][cursor_x], strlen(lines[cursor_y]) - cursor_x);
            }
            lines[cursor_y][cursor_x] = ch;
            cursor_x++;
            // print invalid character
            mvprintw(max_y - 1, 0, "Invalid character: %d", ch);
            break;
        }
    }
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
        if (strcmp(input[0], "vi") == 0)
        {
            run_text_editor(input[1]);
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

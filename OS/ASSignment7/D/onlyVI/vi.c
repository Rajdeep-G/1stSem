#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    run_text_editor(filename);

    return 0;
}

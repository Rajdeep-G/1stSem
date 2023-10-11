#define CTRL_KEY(k) ((k)&0x1f) // Macro to handle Ctrl key combinations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "vi_editor.h"

struct line
{
    char *chars;
    int length;
};

struct buffer
{
    struct line *lines;
    int num_lines;
};
// Global variables
struct buffer text_buffer;
int cursor_x, cursor_y;
int screen_rows, screen_cols;
int num_lines_modified = 0;
int num_words_modified = 0;
int num_chars_modified = 0;

// Function to initialize the text buffer
void init_buffer()
{
    text_buffer.lines = NULL;
    text_buffer.num_lines = 0;
}

// Function to append a new line to the buffer
void append_line(char *str, int length)
{
    text_buffer.lines = realloc(text_buffer.lines, (text_buffer.num_lines + 1) * sizeof(struct line));
    struct line *line = &text_buffer.lines[text_buffer.num_lines];
    line->chars = malloc(length + 1);
    memcpy(line->chars, str, length);
    line->chars[length] = '\0';
    line->length = length;
    text_buffer.num_lines++;
}

// Function to insert a character at the cursor position
void insert_char(char c)
{
    struct line *line = &text_buffer.lines[cursor_y];
    line->chars = realloc(line->chars, line->length + 2);
    memmove(&line->chars[cursor_x + 1], &line->chars[cursor_x], line->length - cursor_x + 1);
    line->chars[cursor_x] = c;
    line->length++;
    cursor_x++;
    num_chars_modified++;
}

// Function to delete the character at the cursor position
void delete_char()
{
    if (cursor_y >= text_buffer.num_lines || cursor_x >= text_buffer.lines[cursor_y].length)
        return;

    struct line *line = &text_buffer.lines[cursor_y];
    memmove(&line->chars[cursor_x], &line->chars[cursor_x + 1], line->length - cursor_x);
    line->length--;
    num_chars_modified--;
}

// Function to save the buffer to a file
void save_buffer(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Unable to save file");
        return;
    }

    for (int i = 0; i < text_buffer.num_lines; i++)
        fprintf(file, "%s\n", text_buffer.lines[i].chars);

    fclose(file);
    printf("Number of lines modified: %d\n", num_lines_modified);
    printf("Number of words modified: %d\n", num_words_modified);
    printf("Number of characters modified: %d\n", num_chars_modified);
}

// Function to draw the screen
void draw_screen()
{
    clear();

    for (int i = 0; i < screen_rows; i++)
    {
        int buffer_row = i;
        if (buffer_row >= text_buffer.num_lines)
        {
            if (text_buffer.num_lines == 0 && i == screen_rows / 3)
            {
                char welcome[80];
                snprintf(welcome, sizeof(welcome), "Text Editor - Press Ctrl+X to save and exit");
                mvprintw(screen_rows / 3, (screen_cols - strlen(welcome)) / 2, welcome);
            }
            else
            {
                mvaddch(i, 0, '~');
            }
        }
        else
        {
            int line_length = text_buffer.lines[buffer_row].length;
            if (cursor_x <= line_length)
            {
                mvprintw(i, 0, &text_buffer.lines[buffer_row].chars[0]);
                if (i == cursor_y)
                {
                    attron(A_REVERSE);
                    mvaddch(i, cursor_x, text_buffer.lines[buffer_row].chars[cursor_x]);
                    attroff(A_REVERSE);
                }
            }
        }
    }

    refresh();
}

// Function to handle key presses
void process_keypress()
{
    int ch = getch();

    switch (ch)
    {
    case KEY_UP:
        if (cursor_y > 0)
            cursor_y--;
        break;
    case KEY_DOWN:
        if (cursor_y < screen_rows - 1)
            cursor_y++;

        break;
    case KEY_LEFT:
        if (cursor_x > 0)
            cursor_x--;

        break;
    case KEY_RIGHT:
        if (cursor_x < screen_cols - 1)
            cursor_x++;

        break;
    case KEY_DC: // Delete key
        delete_char();
        break;
    case '\n': // Enter key
        break;
    case CTRL_KEY('x'):
        save_buffer("output.txt");
        endwin();
        exit(0);
        break;
    default:
        insert_char(ch);
        break;
    }
}

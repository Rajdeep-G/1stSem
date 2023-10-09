// vi_editor.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "vi_editor.h"
#define CTRL_KEY(k) ((k)&0x1f) // Macro to handle Ctrl key combinations

// Structure to represent a single line of text
typedef struct
{
    char *chars;
    int length;
} Line;

// Structure to represent the entire text buffer
typedef struct
{
    Line *lines;
    int num_lines;
} Buffer;

// Global variables
Buffer text_buffer;
int cursor_x, cursor_y;
int screen_rows, screen_cols;

// Function to initialize the text buffer
void init_buffer()
{
    text_buffer.lines = NULL;
    text_buffer.num_lines = 0;
}

// Function to append a new line to the buffer
void append_line(char *str, int length)
{
    text_buffer.lines = realloc(text_buffer.lines, (text_buffer.num_lines + 1) * sizeof(Line));
    Line *line = &text_buffer.lines[text_buffer.num_lines];
    line->chars = malloc(length + 1);
    memcpy(line->chars, str, length);
    line->chars[length] = '\0';
    line->length = length;
    text_buffer.num_lines++;
}

// Function to insert a character at the cursor position
void insert_char(char c)
{
    Line *line = &text_buffer.lines[cursor_y];
    line->chars = realloc(line->chars, line->length + 2);
    memmove(&line->chars[cursor_x + 1], &line->chars[cursor_x], line->length - cursor_x + 1);
    line->chars[cursor_x] = c;
    line->length++;
    cursor_x++;
}

// Function to delete the character at the cursor position
void delete_char()
{
    if (cursor_y >= text_buffer.num_lines || cursor_x >= text_buffer.lines[cursor_y].length)
        return;

    Line *line = &text_buffer.lines[cursor_y];
    memmove(&line->chars[cursor_x], &line->chars[cursor_x + 1], line->length - cursor_x);
    line->length--;
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
    {
        fprintf(file, "%s\n", text_buffer.lines[i].chars);
    }

    fclose(file);
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
                snprintf(welcome, sizeof(welcome), "Text Editor - Press Ctrl+Q to exit");
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
        {
            cursor_y--;
        }
        break;
    case KEY_DOWN:
        if (cursor_y < screen_rows - 1)
        {
            cursor_y++;
        }
        break;
    case KEY_LEFT:
        if (cursor_x > 0)
        {
            cursor_x--;
        }
        break;
    case KEY_RIGHT:
        if (cursor_x < screen_cols - 1)
        {
            cursor_x++;
        }
        break;
    case KEY_DC: // Delete key
        delete_char();
        break;
    case '\x1b': // Escape key (ESC)
        // Close the editor
        endwin();
        exit(0);
        break;
    case CTRL_KEY('s'): // Ctrl+S to save
        save_buffer("output.txt");
        break;
    case CTRL_KEY('x'): // Ctrl+X to exit
        // Close the editor
        endwin();
        exit(0);
        break;
    default:
        // Insert the character at the cursor position
        insert_char(ch);
        break;
    }
}

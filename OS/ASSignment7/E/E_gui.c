#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define MAX_INPUT_LENGTH 100

// Function to execute a shell command and capture its output
void execute_command_and_display_output(Display *d, Window w, GC gc, char *command)
{
      // Redirect the output (stdout and stderr) to a temporary file
      FILE *temp_output = tmpfile();
      if (temp_output == NULL)
      {
            perror("Failed to create temporary file");
            return;
      }

      // Create a pipe to capture command output
      int pipe_fd[2];
      if (pipe(pipe_fd) == -1)
      {
            perror("Failed to create pipe");
            fclose(temp_output);
            return;
      }

      pid_t child_pid = fork();
      if (child_pid == -1)
      {
            perror("Fork failed");
            fclose(temp_output);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            return;
      }

      if (child_pid == 0)
      {
            // Child process
            // Redirect both stdout and stderr to the write end of the pipe
            dup2(pipe_fd[1], STDOUT_FILENO);
            dup2(pipe_fd[1], STDERR_FILENO);
            close(pipe_fd[0]);
            close(pipe_fd[1]);

            // Execute the command
            char *input[MAX_INPUT_LENGTH];
            int count = 0;
            char *token = strtok(command, " ");
            while (token != NULL)
            {
                  input[count++] = token;
                  token = strtok(NULL, " ");
            }
            input[count] = NULL;

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
            // Parent process
            close(pipe_fd[1]);

            // Read the output from the pipe and write it to the temporary file
            char buffer[1024];
            ssize_t n;
            while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
            {
                  fwrite(buffer, 1, n, temp_output);
            }
            close(pipe_fd[0]);

            // Wait for the child process to finish
            int status;
            waitpid(child_pid, &status, 0);

            // Display the output in the GUI window
            fseek(temp_output, 0, SEEK_SET);
            char output[MAX_INPUT_LENGTH];
            size_t output_length = fread(output, 1, sizeof(output), temp_output);
            output[output_length] = '\0';
            XDrawString(d, w, gc, 10, 100, output, strlen(output));
            XFlush(d);

            // Close and remove the temporary file
            fclose(temp_output);
      }
}

int main()
{
      Display *d;
      Window w;
      XEvent e;
      char user_input[MAX_INPUT_LENGTH];

      // Initialize the X display
      d = XOpenDisplay(NULL);
      if (d == NULL)
      {
            fprintf(stderr, "Cannot open display\n");
            exit(1);
      }

      // Create a simple window
      w = XCreateSimpleWindow(d, DefaultRootWindow(d), 10, 10, 1000, 600, 1,BlackPixel(d, 0), WhitePixel(d, 0));
      XSelectInput(d, w, ExposureMask | KeyPressMask);
      XMapWindow(d, w);

      // Create a font and a graphics context for drawing text
      XFontStruct *font_info;
      font_info = XLoadQueryFont(d, "6x13"); // Use the "fixed" font
      GC gc = XCreateGC(d, w, 0, 0);
      XSetFont(d, gc, font_info->fid);
      XSetForeground(d, gc, BlackPixel(d, 0));

      // Event loop
      while (1)
      {
            XNextEvent(d, &e);
            if (e.type == Expose)
            {
                  // Draw the input field
                  XDrawString(d, w, gc, 10, 40, "Enter a command:", 16);
                  // XDrawString(d, w, gc, 10, 60, user_input, strlen(user_input));
                  printf("Inside Expose\n");
                  XFlush(d);
            }
            if (e.type == KeyPress)
            {
                  KeySym key;
                  char text[10];
                  for (int i = 0; i < 10; i++)
                  {
                        text[i] = '\0';
                  }
                  int i = 0;
                  int x = 10;
                  printf("Inside KeyPress\n");
                  while (1)
                  {

                        XLookupString(&e.xkey, text, sizeof(text), &key, 0);
                        if (key == XK_Return || i >= MAX_INPUT_LENGTH - 1)
                        {
                              user_input[i] = '\0'; // Null-terminate the string
                              break;
                        }
                        printf("%c", text[0]);
                        user_input[i++] = text[0];
                   
                  }
                  for (int i = 0; i < MAX_INPUT_LENGTH; i++)
                  {
                        printf("%c", user_input[i]);
                  }
                  // Process user input when Enter is pressed
                  if (key == XK_Return)
                  {
                        //     execute_command_and_display_output(d, w, gc, user_input);
                        // print user_input in the terminal
                        execute_command_and_display_output(d, w, gc, "ls");
                        printf("\nHERE%s\n", user_input);
                        printf("\n%lu\n", sizeof(user_input));
                        // Clear the input field
                        memset(user_input, 0, sizeof(user_input));

                        // Clear the input and output area on the window
                        XClearArea(d, w, 0, 0, 1000, 600, 0);

                        // Draw the input field
                        XDrawString(d, w, gc, 10, 40, "Enter a command:", 16);
                        

                        XFlush(d);
                  }
            }
      }

      // Close the X display
      XCloseDisplay(d);

      return 0;
}

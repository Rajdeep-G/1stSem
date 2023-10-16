#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>

// Dummy function to process user input
void processUserInput(const char *input) {
    printf("User input: %s\n", input);
    // You can perform any desired processing on the input here
}

int main() {
    Display *display;
    Window window;
    XEvent event;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    // Create a window
    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, 400, 100, 1, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    // Set up a font and a graphics context
    XFontStruct *font_info;
    font_info = XLoadQueryFont(display, "6x13");
    GC gc = XCreateGC(display, window, 0, 0);
    XSetFont(display, gc, font_info->fid);
    int font_height = font_info->ascent + font_info->descent;
    XSetForeground(display, gc, BlackPixel(display, screen));

    char user_input[256];
    char temp_input[256]; // Separate buffer to store input
    memset(user_input, 0, sizeof(user_input)); // Initialize the input buffer
    memset(temp_input, 0, sizeof(temp_input)); // Initialize the temp buffer
    int char_no = 0;

    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XDrawString(display, window, gc, 10, 40, "Enter a message:", 16);
            XDrawString(display, window, gc, 10, 60, user_input, strlen(user_input));
            XFlush(display);
        }
        if (event.type == KeyPress) {
            KeySym key;
            char text[2];
            for (int i = 0; i < 2; i++) {
                text[i] = '\0';
            }
            int i = 0;
            while (1) {
                XLookupString(&event.xkey, text, sizeof(text), &key, 0);
                if (key == XK_Return) {
                    // Call the dummy function to process user input
                    processUserInput(user_input);
                    memset(user_input, 0, sizeof(user_input)); // Clear the input buffer
                    XClearWindow(display, window); // Clear the window
                    char_no = 0; // Reset character count
                    break;
                }
                if (i >= sizeof(text) - 1) {
                    break;
                }
                temp_input[i++] = text[0];
            }
            
            // Concatenate the temp input to the user input
            strcat(user_input, temp_input);

            // Update the window to display the user input
            XClearWindow(display, window);
            XDrawString(display, window, gc, 10 , 60, user_input, strlen(user_input));
            XFlush(display);
            char_no++;
        }
    }

    XCloseDisplay(display);
    return 0;
}

#include <stdio.h>

int main() {
    // Open a file for writing
    FILE *file = fopen("output.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write "Hello, World!" to the file
    fprintf(file, "Hello, World!\n");

    // Close the file
    fclose(file);

    return 0;
}


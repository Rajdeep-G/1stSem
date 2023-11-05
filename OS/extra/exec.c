// execv() System Function:
// In execl() function, the parameters of the executable file is passed to the function as different arguments. With execv(), you can pass all the parameters in a NULL terminated array argv. The first element of the array should be the path of the executable file. Otherwise, execv() function works just as execl() function.

// Syntax:
int execv(const char *path, char *const argv[]);
// We can rewrite the execl() example as follows:

#include <unistd.h>
 
int main(void) {
  char *binaryPath = "/bin/ls";
  char *args[] = {binaryPath, "-lh", "/home", NULL};
 
  execv(binaryPath, args);
 
  return 0;
}

// execvp() System Function:
// Works the same way as execv() system function. But, the PATH environment variable is used. So, the full path of the executable file is not required just as in execlp().

// Syntax:
int execvp(const char *file, char *const argv[]);
// We can rewrite the execv() example as follows:

#include <unistd.h>
 
int main(void) {
  char *programName = "ls";
  char *args[] = {programName, "-lh", "/home", NULL};
 
  execvp(programName, args);
 
  return 0;
}

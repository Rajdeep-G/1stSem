//execDemo.c

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
		//A null terminated array of character 
		//pointers
		char *args[]={"./exec",NULL};
		execvp(args[0],args);
	
		/*All statements are ignored after execvp() call as this whole 
		process(execDemo.c) is replaced by another process (EXEC.c)
		*/
		printf("Ending-----");
	
	return 0;
}


// When the file execDemo.c is compiled, as soon as the statement execvp(args[0],args) 
// is executed, this very program is replaced by the program EXEC.c. 
// “Ending—–” is not printed because as soon as the execvp() function is called, 
// this program is replaced by the program EXEC.c.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

main()
{
	char GeeksforGeeks1, GeeksforGeeks2;
	char *message1 = "GeeksforGeeks 1";
	char *message2 = "GeeksforGeeks 2";
	int geeky1, geeky2;

	geeky1 = pthread_create( &GeeksforGeeks1, NULL, print_message_function, (void*) message1);
	geeky2 = pthread_create( &GeeksforGeeks2, NULL, print_message_function, (void*) message2);

	pthread_join( GeeksforGeeks1, NULL);
	pthread_join( GeeksforGeeks2, NULL); 

	printf("GeeksforGeeks 1 returns: %d\n",geeky1);
	printf("GeeksforGeeks 2 returns: %d\n",geeky2);
	exit(0);
}

void *print_message_function( void *ptr )
{
	char *message;
	message = (char *) ptr;
	printf("%s \n", message);
}


// .............................................................................................................................................................................
// C program to show thread functions 

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 

void* func(void* arg) 
{ 
	// detach the current thread 
	// from the calling thread 
	pthread_detach(pthread_self()); 

	printf("Inside the thread\n"); 

	// exit the current thread 
	pthread_exit(NULL); 
} 

void fun() 
{ 
	pthread_t ptid; 

	// Creating a new thread 
	pthread_create(&ptid, NULL, &func, NULL); 
	printf("This line may be printed"
		" before thread terminates\n"); 

	// The following line terminates 
	// the thread manually 
	// pthread_cancel(ptid); 

	// Compare the two threads created 
	if(pthread_equal(ptid, pthread_self())) 
		printf("Threads are equal\n"); 
	else
		printf("Threads are not equal\n"); 

	// Waiting for the created thread to terminate 
	pthread_join(ptid, NULL); 

	printf("This line will be printed"
		" after thread ends\n"); 

	pthread_exit(NULL); 
} 

// Driver code 
int main() 
{ 
	fun(); 
	return 0; 
} 

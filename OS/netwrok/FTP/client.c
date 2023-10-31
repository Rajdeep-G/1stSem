#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 22
void handle_ftp()
{
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];
    int n;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (client_socket < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("Connected to the server.\n");
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);

    int choice = 0;
    printf("1. get\n2. put\n3. quit\n4.cd\n5.ls\n");
    scanf("%d", &choice);
    if (choice == 1)
    {
        char get_command[] = "get f1.txt\r\n";
        send(client_socket, get_command, strlen(get_command), 0);
        // receive the file size
        n = recv(client_socket, buffer, sizeof(buffer), 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }

        // receive the file chunks
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE of chunk size%s\n", buffer);
        }

        // // receive the file
        FILE *file = fopen("f1_down.txt", "wb");
        if (file == NULL)
        {
            perror("[-]Error in creating file.");
            exit(1);
        }
        int no_chunk = atoi(buffer);

        // create a buffer to store the content of all the chunks of the file
        char *file_buffer = (char *)malloc(no_chunk * MAX_BUFFER_SIZE * sizeof(char));
        int i = 0;
        while (i < no_chunk)
        {
            n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
            if (n > 0)
            {
                buffer[n] = '\0';
                // printf("SERVER RESPONSE %s\n", buffer);
                strcat(file_buffer, buffer);
            }
            i++;
        }
        fwrite(file_buffer, sizeof(char), strlen(file_buffer), file);

        fclose(file);
        printf("[+]File transfer complete.\n");

        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }
    }
    else if (choice == 2)
    {

        char put_command[] = "put f2.txt\r\n";
        send(client_socket, put_command, strlen(put_command), 0);

        FILE *file = fopen("f2.txt", "rb");
        if (file == NULL)
        {
            perror("[-]Error in reading file.");
            exit(1);
        }
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }

        // add EOF to the end of file
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        fseek(file, file_size, SEEK_SET);
        fwrite("EOF", sizeof(char), 3, file);
        fseek(file, 0, SEEK_SET);
        // send file size
        sprintf(buffer, "%ld", file_size);
        send(client_socket, buffer, strlen(buffer), 0);

        while ((n = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0)
        {
            send(client_socket, buffer, n, 0);
        }
        fclose(file);

        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }
    }
    else if (choice == 3)
    {
        char quit_command[] = "quit\r\n";
        send(client_socket, quit_command, strlen(quit_command), 0);
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }
    }
    else if (choice == 4)
    {
        char cd_command[] = "cd t1\r\n";
        send(client_socket, cd_command, strlen(cd_command), 0);
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }
    }
    else if (choice == 5)
    {
        char ls_command[] = "ls\r\n";
        send(client_socket, ls_command, strlen(ls_command), 0);
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("SERVER RESPONSE %s\n", buffer);
        }
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        // if (n > 0)
        // {
        //     buffer[n] = '\0';
        //     printf("SERVER RESPONSE of chunk size %s\n", buffer);
        // }
        char msg[] = "1";
        send(client_socket, msg, strlen(msg), 0);

        int no_chunk = atoi(buffer);
        char *file_buffer = (char *)malloc(no_chunk * MAX_BUFFER_SIZE * sizeof(char));
        int i = 0;
        while (i < no_chunk)
        {
            n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
            if (n > 0)
            {
                buffer[n] = '\0';
                
                strcat(file_buffer, buffer);
            }
            i++;
        }
        printf("SERVER RESPONSE of \"ls\" \n%s\n", file_buffer);
        
        printf("Finished\n");
    }
    else
    {
        printf("Invalid choice\n");
        exit(1);
    }
    close(client_socket);
    printf("Disconnected from the server.\n");
}

//
int main(int argc, char const *argv[])
{
    handle_ftp();
    return 0;
}

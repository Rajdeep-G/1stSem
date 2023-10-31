#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

#define TIMEOUT_SEC 10 // Timeout value in seconds
#define PORT 5566
#define IP "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

void handle_FTP(void *arg)
{
    int client_socket = *(int *)arg;
    char buffer[MAX_BUFFER_SIZE];
    int n;

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    char welcome_msg[] = "220 Anonymous FTP server ready.\r\n";
    send(client_socket, welcome_msg, strlen(welcome_msg), 0);
    while (1)
    {
        n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (n <= 0)
        {
            printf("[-]Client disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("[+]Receive from client: %s\n", buffer);
        if (strstr(buffer, "put") == buffer)
        {
            char *filename = strtok(buffer, " ");
            filename = strtok(NULL, " ");
            printf("[+]Receive file name: %s\n", filename);
            // modify the file name to filename+servercreated
            char *new_filename = malloc(strlen(filename) + 20);
            strcpy(new_filename, filename);
            new_filename[strlen(new_filename) - 1] = '\0';
            strcat(new_filename, "_servercreated");

            FILE *file = fopen(new_filename, "wb");
            if (file == NULL)
            {
                char response[] = "550 Failed to open file for writing\r\n";
                send(client_socket, response, strlen(response), 0);
            }
            else
            {
                char response[] = "150 File OK\r\n";
                send(client_socket, response, strlen(response), 0);
                // receive the file size
                n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
                buffer[n] = '\0';
                int file_size = atoi(buffer);
                printf("[+]Receive file size: %d\n", file_size);
                int no_chunk = file_size / MAX_BUFFER_SIZE;
                // round up
                if (file_size % MAX_BUFFER_SIZE != 0)
                    no_chunk++;

                // printf("[+]Receive number of chunks: %d\n", no_chunk);
                while (n = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0))
                {
                    if (n <= 0)
                    {
                        printf("[-]Client disconnected.\n");
                        break;
                    }
                    buffer[n] = '\0';
                    // printf("[+]Receive from client: %s\n", buffer);
                    fwrite(buffer, sizeof(char), n, file);
                    no_chunk--;
                    if (no_chunk == 0)
                    {
                        break;
                    }
                }
                printf("[+]File transfer complete.\n");
                fclose(file);
                char response2[] = "226 Transfer complete\r\n";
                send(client_socket, response2, strlen(response2), 0);
            }
        }

        else if ((strstr(buffer, "get") == buffer))
        {
            char *filename = strtok(buffer, " ");
            filename = strtok(NULL, " ");
            printf("[+]Receive file name: %s\n", filename);

            // get the file name and store in a new variable and open it
            char *new_filename = malloc(strlen(filename) + 20);
            strcpy(new_filename, filename);
            new_filename[strlen(new_filename) - 1] = '\0';

            FILE *file = fopen("f1.txt", "rb");

            if (file == NULL)
            {
                perror("[-]Error in reading file.");
                exit(1);
            }
            else
            {
                printf("[+]File opened succ.\n");
                char response[] = "150 File OK\r\n";
                send(client_socket, response, strlen(response), 0);
                // send the file size
                fseek(file, 0, SEEK_END);
                long file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                // fseek(file, file_size, SEEK_SET);
                // fwrite("EOF", sizeof(char), 3, file);
                fseek(file, 0, SEEK_SET);

                int no_chunk = file_size / MAX_BUFFER_SIZE;
                // round up
                if (file_size % MAX_BUFFER_SIZE != 0)
                    no_chunk++;
                printf("[+]Send number of chunks: %d\n", no_chunk);
                // send the no of chunks
                sprintf(buffer, "%d", no_chunk);
                send(client_socket, buffer, strlen(buffer), 0);

                printf("\n");

                while ((n = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0)
                {
                    send(client_socket, buffer, n, 0);
                }

                fclose(file);
                char response2[] = "226 Transfer complete\r\n";
                send(client_socket, response2, strlen(response2), 0);
            }
        }

        close(client_socket);
        pthread_exit(NULL);
    }
}
int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // char *ip = "127.0.0.1";
    char *ip = IP;
    int port = PORT;
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(23); // Port 21 for FTP
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("[+]Client connected.\n");

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_FTP, (void *)&client_socket) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    close(server_socket);
}

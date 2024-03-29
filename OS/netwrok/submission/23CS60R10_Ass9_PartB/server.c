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
            new_filename[strlen(new_filename) - 6] = '\0';
            strcat(new_filename, "_uploaded.txt");

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
            char *newline = strchr(filename, '\r');
            if (newline != NULL)
            {
                *newline = '\0';
            }
            FILE *file = fopen(filename, "rb");

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

        else if ((strstr(buffer, "quit") == buffer))
        {
            char response[] = "221 Goodbye\r\n";
            send(client_socket, response, strlen(response), 0);
            break;
        }

        else if ((strstr(buffer, "ls") == buffer))
        {
            char response[] = "150 Here comes the directory listing.\r\n";
            send(client_socket, response, strlen(response), 0);
            system("ls > ls.txt");
            FILE *file = fopen("ls.txt", "rb");

            if (file == NULL)
            {
                perror("[-]Error in reading file.");
                exit(1);
            }
            else
            {

                fseek(file, 0, SEEK_END);
                long file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                fseek(file, 0, SEEK_SET);

                int no_chunk = file_size / MAX_BUFFER_SIZE;
                // round up
                if (file_size % MAX_BUFFER_SIZE != 0)
                    no_chunk++;
                printf("[+]Send number of chunks: %d\n", no_chunk);
                // send the no of chunks
                char buffer2[10];
                sprintf(buffer2, "%d", no_chunk);

                send(client_socket, buffer2, strlen(buffer2), 0);

                n = read(client_socket, buffer, MAX_BUFFER_SIZE);
                buffer[n] = '\0';
                printf("buffer: %s\n", buffer);

                while ((n = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0)
                {
                    send(client_socket, buffer, n, 0);
                }

                fclose(file);
                system("rm ls.txt");
            }
        }

        else if ((strstr(buffer, "cd") == buffer))
        {
            // change the directory to the specified directory
            char *directory = strtok(buffer, " ");
            directory = strtok(NULL, " ");
            char *newline = strchr(directory, '\r');
            if (newline != NULL)
            {
                *newline = '\0';
            }
            printf("[+]Receive directory name: %s\n", directory);
            int check = chdir(directory);
            if (check == 0)
            {
                char response[] = "250 Directory successfully changed.\r\n";
                send(client_socket, response, strlen(response), 0);
                // system("pwd > pwd.txt"); //just for checking
            }
            else
            {
                char response[] = "550 Failed to change directory.\r\n";
                send(client_socket, response, strlen(response), 0);
            }
        }

        else
        {
            char response[] = "500 Syntax error, command unrecognized\r\n";
            send(client_socket, response, strlen(response), 0);
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

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(21); // Port 21 for FTP
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

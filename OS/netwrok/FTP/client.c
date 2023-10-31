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
#define SERVER_PORT 21
void send_ping_request(const char *ip, int port, int num_req, int interval)
{
    int client_socket;
    // char *ip = "127.0.0.1";
    // int port = 5566;
    struct sockaddr_in server_address;
    struct timespec start_time, end_time;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ip);

    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (client_socket < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("Connected to the server.\n");

    while (num_req--)
    {
        bzero(buffer, 1024);
        const char *ping_request = "Ping request";

        clock_gettime(CLOCK_MONOTONIC, &start_time);

        send(client_socket, ping_request, strlen(ping_request), 0);
        recv(client_socket, buffer, sizeof(buffer), 0);

        clock_gettime(CLOCK_MONOTONIC, &end_time);

        double start_time_in_seconds = start_time.tv_sec + start_time.tv_nsec / 1e9;
        double end_time_in_seconds = end_time.tv_sec + end_time.tv_nsec / 1e9;
        double rtt = end_time_in_seconds - start_time_in_seconds;
        printf("Received: %s, RTT: %.6f seconds\n", buffer, rtt);
        sleep(interval);
    }

    close(client_socket);
    printf("Disconnected from the server.\n");
}

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
    close(client_socket);
    printf("Disconnected from the server.\n");
}

//
int main(int argc, char const *argv[])
{
    handle_ftp();
    return 0;
}

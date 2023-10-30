#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

// #define PORT 8080
#define MAX_BUFFER_SIZE 1024

void send_ping_request()
{
    int client_socket;
    char *ip = "127.0.0.1";
    int port = 5566;
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
    close(client_socket);
    printf("Disconnected from the server.\n");
}

int main()
{
    const char *server_ip = "127.0.0.1"; // Hardcoded server IP

    send_ping_request();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <sys/file.h> // For file locking
#include <fcntl.h>    // For file locking

#define TIMEOUT_SEC 5 // Timeout value in seconds
#define PORT 5566
#define IP "127.0.0.1"
#define MAX_BUFFER_SIZE 1024
#define LOG_FILE "ping_log.txt"

void log_ping_request(char *client_ip, int client_port, double rtt)
{
    printf("Client IP: %s  Client port: %d  RTT: %.6f seconds\n", client_ip, client_port, rtt);
    
    // open the file and write to it and use locks
    int fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (flock(fd, LOCK_EX) == -1)
    {
        perror("flock");
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_BUFFER_SIZE];
    sprintf(buffer, "Client IP: %s  Client port: %d  RTT: %.6f seconds\n", client_ip, client_port, rtt);
    write(fd, buffer, strlen(buffer));
    if (flock(fd, LOCK_UN) == -1)
    {
        perror("flock");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
}

void handle_ping_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    int n;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    struct timespec start_time, end_time;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Get the client's address information
    if (getpeername(client_socket, (struct sockaddr *)&client_address, &client_address_len) == -1)
    {
        perror("getpeername");
        return;
    }

    char client_ip[INET_ADDRSTRLEN];
    int client_port;

    // Convert IP address from binary to string
    inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
    client_port = ntohs(client_address.sin_port);

    while (1)
    {
        // setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        n = recv(client_socket, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            // printf("Timeout or packet loss, retransmitting acknowledgment\n");
            // send(client_socket, buffer, n, 0);
            // // continue;
            return;
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        send(client_socket, buffer, n, 0);

        double start_time_in_seconds = start_time.tv_sec + start_time.tv_nsec / 1e9;
        double end_time_in_seconds = end_time.tv_sec + end_time.tv_nsec / 1e9;
        double rtt = end_time_in_seconds - start_time_in_seconds;
        log_ping_request(client_ip, client_port, rtt);
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

    // Bind socket to the specified port
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ip);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }

    // Listen for incoming connections
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

        // handle_ping_request(client_socket, client_address);
        handle_ping_request(client_socket);
        // pthread_t thread;
        // pthread_create(&thread, NULL, handle_ping_request, (void *)&client_socket);
        printf("[+]Client disconnected.\n");
        close(client_socket);
    }
    close(server_socket);
}

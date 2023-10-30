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

void handle_ping_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    int n;
    time_t start_time, end_time;

    n = recv(client_socket, buffer, sizeof(buffer), 0);
    if (n <= 0)
    {
        perror("recv");
        return;
    }

    // time(&start_time);

    // Acknowledge the ping request
    send(client_socket, buffer, n, 0);

    // time(&end_time);

    // double rtt = difftime(end_time, start_time);
    //
    // printf("Received: %s, RTT: %.2f seconds\n", buffer, rtt);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    char *ip = "127.0.0.1";
    int port = 5566;
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

    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("[+]Client connected.\n");
    handle_ping_request(client_socket);

    close(client_socket);
    close(server_socket);

}

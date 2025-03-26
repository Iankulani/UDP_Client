#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    // User enters the server's IP address
    printf("Enter server IP address: ");
    char server_ip[16];
    scanf("%s", server_ip);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        exit(1);
    }

    // User enters the server's port number
    printf("Enter server port number: ");
    int server_port;
    scanf("%d", &server_port);
    server_addr.sin_port = htons(server_port);

    // Send message to server
    printf("Enter a message to send to the server: ");
    getchar();  // Consume the newline left by the previous input
    fgets(buffer, BUFFER_SIZE, stdin);

    if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto failed");
        exit(1);
    }

    // Receive response from server
    socklen_t addr_len = sizeof(server_addr);
    int recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len == -1) {
        perror("recvfrom failed");
        exit(1);
    }

    buffer[recv_len] = '\0';  // Null-terminate the received data
    printf("Server response: %s\n", buffer);

    close(client_socket);
    return 0;
}

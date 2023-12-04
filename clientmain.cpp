#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Included to get the support library
#include "calcLib.h"
#include "protocol.h"

int main(int argc, char *argv[]) {
  // Check command-line arguments for server address and port
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
    return 1;
  }

  const char *serverIP = argv[1];
  int serverPort = atoi(argv[2]);

  // Create a socket for communication with the server
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Socket creation failed");
    return 1;
  }

  // Define the server address
  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(serverPort);
  if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
    perror("Invalid server IP address");
    close(sockfd);
    return 1;
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      -1) {
    perror("Connection to server failed");
    close(sockfd);
    return 1;
  }

  // Initialization of the calcLib
  initCalcLib();

  // Main chat loop
  while (1) {
    char message[256];
    printf("Enter a message (or 'exit' to quit): ");
    fgets(message, sizeof(message), stdin);

    // Check if the user wants to exit
    if (strncmp(message, "exit", 4) == 0) {
      break;
    }

    // Send the message to the server
    if (send(sockfd, message, strlen(message), 0) == -1) {
      perror("Message sending failed");
      break;
    }

    // Receive and display the server's response
    char serverResponse[256];
    ssize_t bytesRead = recv(sockfd, serverResponse, sizeof(serverResponse), 0);
    if (bytesRead <= 0) {
      perror("Message receiving failed");
      break;
    }

    serverResponse[bytesRead] = '\0';
    printf("Server says: %s", serverResponse);
  }

  // Close the socket and clean up
  close(sockfd);
  printf("Chat session ended.\n");

  return 0;
}

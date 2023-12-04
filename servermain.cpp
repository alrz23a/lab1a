#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

// Included to get the support library
#include "calcLib.h"
#include "protocol.h"

using namespace std;
/* Needs to be global, to be reachable by callback and main */
int loopCount = 0;
int terminate = 0;

/* Call back function, will be called when the SIGALRM is raised when the timer
 * expires. */
void checkJobbList(int signum) {
  // As anybody can call the handler, it's good coding to check the signal
  // number that called it.

  printf("Let me be, I want to sleep, loopCount = %d.\n", loopCount);

  if (loopCount > 20) {
    printf("I had enough.\n");
    terminate = 1;
  }

  return;
}

int main(int argc, char *argv[]) {

  /* Do more magic */

  /*
     Prepare to set up a recurring event every 10s. If it_interval, or it_value
     is omitted, it will be a single alarm 10s after it has been set.
  */
  struct itimerval alarmTime;
  alarmTime.it_interval.tv_sec = 10;
  alarmTime.it_interval.tv_usec = 10;
  alarmTime.it_value.tv_sec = 10;
  alarmTime.it_value.tv_usec = 10;

  /* Register a callback function, associated with the SIGALRM signal, which
   * will be raised when the alarm goes off */
  signal(SIGALRM, checkJobbList);
  setitimer(ITIMER_REAL, &alarmTime, NULL); // Start/register the alarm.

  // Create a socket for the server to listen on
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    perror("Socket creation failed");
    return 1;
  }

  // Define the server address
  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345); // You can change the port as needed
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the server address
  if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      -1) {
    perror("Binding failed");
    close(serverSocket);
    return 1;
  }

  // Start listening for incoming connections
  if (listen(serverSocket, 5) == -1) {
    perror("Listening failed");
    close(serverSocket);
    return 1;
  }

  printf("Server is listening for incoming connections...\n");

  while (terminate == 0) {
    // Accept incoming connections
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket =
        accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
      perror("Accepting connection failed");
      continue;
    }

    printf("Accepted a connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr),
           ntohs(clientAddr.sin_port));

    char buffer[256];
    ssize_t bytesRead;

    // Echo received messages back to the client
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      buffer[bytesRead] = '\0';
      printf("Received: %s", buffer);

      // Echo back to the client
      send(clientSocket, buffer, bytesRead, 0);
    }

    // Close the client socket
    close(clientSocket);
  }

  // Close the server socket and clean up
  close(serverSocket);
  printf("Server terminated.\n");

  return 0;
}

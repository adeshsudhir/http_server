/* server.c */

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

  int server_fd;

  /* Creating a communication end point */
  /* AF_INET - IPv4, SOCK_STREAM - Reliable, ordered, connection (TCP), 0 -
   * Picks TCP Automatically*/
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    perror("socket failed");
    exit(1);
  }
  printf("Socket Created\n");

  int opt = 1;

  /* Allow to reuse the port quickly */
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  /* Alloactes memory for storing: IP addr, Port, Addr family */
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));

  /* Set address family - Use IPv4 */
  address.sin_family = AF_INET;
  /* Set IP address - Bind to all available network interfaces */
  /* Server will accept connections from local host, Lan ip, any other interface
   */
  address.sin_addr.s_addr = INADDR_ANY;
  /* Set port number */
  /* why htons - converts host byte order -> network byte order (big endian) */
  address.sin_port = htons(PORT);

  /* Attach this socket to port */
  /* The output will be 0.0.0.0:PORT */
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(1);
  }
  printf("Bound to port %d", PORT);

  /* Start accepting incoming connection requests */
  /* here 10 means max 10 pending connections waiting to be accepted */
  if (listen(server_fd, 10)) {
    perror("listen failed");
    exit(1);
  }
  printf("Listening... test with: nc localhost %d\n\n", PORT);

  while (1) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("Waiting for connection... \n");

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
      perror("accept failed");
      continue;
    }

    printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
      close(client_fd);
      continue;
    }

    printf("Received: \n%s\n", buffer);

    char response[BUFFER_SIZE + 64];
    snprintf(response, sizeof(response), "Echo: %s\n", buffer);
    printf("Echoed back to client\n");

    write(client_fd, response, strlen(response));
    printf("Echoed back to client\n");

    close(client_fd);
    printf("Connection closed\n\n");
  }
}

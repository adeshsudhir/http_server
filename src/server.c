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

  /* Assign  socket -> port */
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(1);
  }
  printf("Bound to port %d", PORT);

  if (listen(server_fd, 10)) {
    perror("listen failed");
    exit(1);
  }
  printf("Listening... test with: nc localhost %d\n\n", PORT);
}

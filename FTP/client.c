#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int sock_fd;
  struct sockaddr_in server;
  char filename[20], filedata[300];

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd < 0) {
    perror("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 0) {
    perror("Error!\n");
    return -1;
  }

  if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Client-Server Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  printf("Enter the name of the file: ");
  scanf("%s", filename);
  int filenamelen = strlen(filename);

  write(sock_fd, filename, filenamelen);
  int rdwrt = read(sock_fd, filedata, 300);

  printf("The contents of the file %s is: \n%s", filename, filedata);

  return 0;
}

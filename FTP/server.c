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
  int server_fd, client_fd, clientlen;
  struct sockaddr_in client, server;
  char filename[100], filedata[300];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Socket Binded Successfully\n");

  if (listen(server_fd, 3) < 0) {
    printf("Listening Failed!\n");
    return -1;
  }
  printf("Listening...\n");

  clientlen = sizeof(client);

  client_fd =
      accept(server_fd, (struct sockaddr *)&client, (socklen_t *)&clientlen);

  if (client_fd < 0) {
    printf("Client-Server Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  int n = read(client_fd, filename, sizeof(filename));
  filename[n] = '\0';

  printf("The requested file from the client is: %s\n", filename);

  int file = open(filename, O_RDWR);

  if (file < 0) {
    perror("File Opening Failed!\n");
    strcpy(filedata, "File not found on server!");
    write(client_fd, filedata, sizeof(filedata));
  } else {
    n = read(file, filedata, sizeof(filedata) - 1);
    filedata[n] = '\0';
    write(client_fd, filedata, sizeof(filedata));
    close(file);
  }

  close(client_fd);
  close(server_fd);

  return 0;
}

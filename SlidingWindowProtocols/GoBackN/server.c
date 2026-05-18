#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8086

struct timeval timeout;

void func(int connfd) {
  char buff[MAX];
  int frame, c, ack, next = 0;

  while (1) {
    sleep(1);
    bzero(buff, MAX);
    recv(connfd, buff, MAX, 0);

    if (strncmp(buff, "Exit", 4) == 0) {
      printf("Exit!\n");
      break;
    }
    frame = atoi(buff);
    if (frame != next) {
      printf("Frame %d discarded!\nAcknowledgement sent: %d\n", frame, ack);
      bzero(buff, MAX);
      snprintf(buff, sizeof(buff), "%d", ack);
      send(connfd, buff, sizeof(buff), 0);
      continue;
    }
    c = rand() % 3;

    switch (c) {
    case 0:
      break;

    case 1:
      ack = frame;
      sleep(2);
      printf("Frame %d Received\nAcknowledgement sent: %d\n", frame, ack);
      bzero(buff, MAX);
      snprintf(buff, sizeof(buff), "%d", ack);
      send(connfd, buff, sizeof(buff), 0);
      next = ack + 1;
      break;

    case 2:
      ack = frame;
      printf("Frame %d Received\nAcknowledgement sent: %d\n", frame, ack);
      bzero(buff, MAX);
      snprintf(buff, sizeof(buff), "%d", ack);
      send(connfd, buff, sizeof(buff), 0);
      next = ack + 1;
      break;
    }
  }
}

int main() {
  int sock_fd, conn_fd;
  struct sockaddr_in server, client;
  int clientlen = sizeof(client);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT);

  if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binded Successfully!\n");

  if (listen(sock_fd, 3) < 0) {
    printf("Listening Failed!\n");
    return -1;
  }
  printf("Listening...\n");

  conn_fd =
      accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&clientlen);

  if (conn_fd < 0) {
    printf("Client Server Communication Terminated!\n");
    return -1;
  }
  printf("Client Server Connection Established!\n");

  func(conn_fd);
  close(sock_fd);
}

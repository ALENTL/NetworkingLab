#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

void func(int connfd);

int main() {
  int sockfd, newsock;
  struct sockaddr_in server;
  int clientlen = sizeof(server);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)clientlen) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binding Successful!\n");

  if (listen(sockfd, 3) < 0) {
    printf("Listening Failed!\n");
    return -1;
  }
  printf("Listening! ... \n");

  newsock = accept(sockfd, (struct sockaddr *)&server, (socklen_t *)&clientlen);

  if (newsock < 0) {
    printf("Client-Server Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  func(newsock);
  close(newsock);
  close(sockfd);

  return 0;
}

void func(int connfd) {
  char buff[80];
  int win = 8, frames, lostFrame, ack;

  srand(time(0));
  read(connfd, buff, sizeof(buff));

  if (strcmp("end", buff) == 0) {
    printf("Exit\n");
    return;
  }

  frames = atoi(buff);
  lostFrame = rand() % frames;

  for (int k = 0; k < frames; k++) {
    if (k != lostFrame) {
      sleep(1);
      printf("\nReceived Frame: %d", k);
    }
  }
  sleep(1);
  printf("\n\nFrame %d not received\nWaiting for Frame %d\n", lostFrame,
         lostFrame);

  ack = lostFrame;
  snprintf(buff, sizeof(buff), "%d", ack);
  write(connfd, buff, sizeof(buff));

  read(connfd, buff, sizeof(buff));

  if (strcmp("end", buff) == 0) {
    printf("Exit\n");
    return;
  }

  frames = atoi(buff);
  sleep(1);
  printf("\nReceived Retransmitted Frame: %d\n", frames);

  strcpy(buff, "end");
  write(connfd, buff, sizeof(buff));
}

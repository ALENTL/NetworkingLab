#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void func(int sockfd);

int main() {
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 0) {
    printf("Invalid Address / Address not Supported!\n");
    return -1;
  }

  if (connect(sockfd, (struct sockaddr *)&server, (socklen_t)sizeof(server)) <
      0) {
    printf("Client-Server Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  func(sockfd);
  close(sockfd);

  return 0;
}

void func(int sockfd) {
  char buff[80];
  int ack;
  int frames = 8;

  printf("Sending Frames: ");
  for (int i = 0; i < frames; i++) {
    printf("%d ", i);
  }
  printf("\n");

  snprintf(buff, sizeof(buff), "%d", frames);
  write(sockfd, buff, sizeof(buff));

  read(sockfd, buff, sizeof(buff));
  ack = atoi(buff);

  printf("NAK received for frame: %d\n", ack);
  printf("Retransmitting Frame: %d\n", ack);

  snprintf(buff, sizeof(buff), "%d", ack);
  write(sockfd, buff, sizeof(buff));

  read(sockfd, buff, sizeof(buff));

  if (strcmp("end", buff) == 0) {
    printf("All Frames Sent Successfully!\n");
    printf("Exit!\n");
  }
}

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8086

struct timeval timeout;

void func(int sock_fd, int frames, int windowSize) {
  char buff[MAX];
  int ack, windowStart = 0, windowEnd = windowSize - 1, flag = 0, i, readval;

  if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                 sizeof(timeout)) < 0) {
    perror("setsockopt(SO_RCVTIMEO) failed!\n");
  }

  for (i = 0; i < frames && i <= windowEnd; i++) {
    bzero(buff, MAX);
    snprintf(buff, MAX, "%d", i);
    send(sock_fd, buff, MAX, 0);
    printf("Frame %d sent\n", i);
  }

  while (1) {
    if (windowEnd - windowStart != windowSize - 1 && flag == 0 && i != frames) {
      bzero(buff, MAX);
      snprintf(buff, MAX, "%d", i);
      send(sock_fd, buff, MAX, 0);
      printf("Frame %d sent\n", i);
      windowEnd++, i++;
    }
    flag = 0;
    bzero(buff, MAX);
    readval = recv(sock_fd, buff, MAX, 0);
    ack = atoi(buff);

    if (readval > 0) {
      if (ack + 1 == frames) {
        printf("Acknowledgement Received: %d\nExit\n", ack);
        bzero(buff, MAX);
        strcpy(buff, "Exit\n");
        send(sock_fd, buff, MAX, 0);
        break;
      }

      if (ack == windowStart) {
        windowStart++;
        printf("Acknowledgement Received: %d\n", ack);
      }
    } else {
      printf("Acknowledgement not received for %d\nResending frames\n",
             windowStart);

      for (int j = windowStart; j < frames && j < windowStart + windowSize;
           j++) {
        bzero(buff, MAX);
        snprintf(buff, MAX, "%d", j);
        send(sock_fd, buff, MAX, 0);
        printf("Frame %d sent\n", j);
      }
      flag = 1;
    }
  }
}

int main() {
  int sock_fd, frames, windowSize;
  struct sockaddr_in server;

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
  }

  timeout.tv_sec = 3;
  timeout.tv_usec = 0;

  if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Connection to the server failed!\n");
    return -1;
  }
  printf("Connection Established Successfully!\n");

  printf("Enter the number of frames: ");
  scanf("%d", &frames);

  printf("Enter the window size: ");
  scanf("%d", &windowSize);

  func(sock_fd, frames, windowSize);
  close(sock_fd);

  return 0;
}

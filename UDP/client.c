#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sock_fd;
  struct sockaddr_in server;
  int clientlen = sizeof(server);

  int row = 2, col = 2, matrixA[row][col], matrixB[row][col], flat[4];

  sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);

  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

  printf("Matrix A: \n");
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("A[%d][%d]: ", i + 1, j + 1);
      scanf("%d", &matrixA[i][j]);
    }
  }

  int k = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      flat[k++] = matrixA[i][j];
    }
  }

  sendto(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&server, clientlen);

  printf("Matrix B: \n");
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("B[%d][%d]: ", i + 1, j + 1);
      scanf("%d", &matrixB[i][j]);
    }
  }

  k = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      flat[k++] = matrixB[i][j];
    }
  }

  sendto(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&server, clientlen);

  recvfrom(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&server,
           (socklen_t *)&clientlen);

  k = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d\t", flat[k++]);
    }
    printf("\n");
  }
  close(sock_fd);

  return 0;
}

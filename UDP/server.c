#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sock_fd;
  struct sockaddr_in client, server;
  int clientlen = sizeof(client);

  int row = 2, col = 2, flat[4], matrixA[row][col], matrixB[row][col],
      resultMatrix[row][col];

  sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Succesfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binded Succesfully!\n");

  if (recvfrom(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&client,
               (socklen_t *)&clientlen) < 0) {
    printf("Receiving Failed!\n");
  }

  int k = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrixA[i][j] = flat[k++];
    }
  }

  if (recvfrom(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&client,
               (socklen_t *)&clientlen) < 0) {
    printf("Receiving Failed!\n");
  }

  k = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrixB[i][j] = flat[k++];
    }
  }

  k = 0;
  printf("Matrix Addition (Server Side): \n");
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      resultMatrix[i][j] = matrixA[i][j] + matrixB[i][j];
      printf("%d\t", resultMatrix[i][j]);
      flat[k++] = resultMatrix[i][j];
    }
    printf("\n");
  }

  if (sendto(sock_fd, flat, sizeof(flat), 0, (struct sockaddr *)&client,
             clientlen) < 0) {
    printf("Sending Failed!\n");
    return -1;
  }

  printf("Result sent to the client successfully!\n");
  close(sock_fd);

  return 0;
}

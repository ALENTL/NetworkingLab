#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socdef;
  struct sockaddr_in server;
  int clientlen = sizeof(server);
  int r = 2, c = 2, a[100][100], b[100][100], flat[100];

  socdef = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (socdef < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 0) {
    printf("Error\n");
    return -1;
  }

  printf("Enter the elements for Matrix A: \n");
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      printf("Enter the element A[%d][%d]: ", i + 1, j + 1);
      scanf("%d", &a[i][j]);
    }
  }

  int k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      flat[k++] = a[i][j];
    }
  }

  sendto(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&server, clientlen);

  printf("Enter the elements for Matrix B: \n");
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      printf("Enter the element A[%d][%d]: ", i + 1, j + 1);
      scanf("%d", &b[i][j]);
    }
  }

  k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      flat[k++] = b[i][j];
    }
  }

  sendto(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&server, clientlen);

  if (recvfrom(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&server,
               (socklen_t *)&clientlen) < 0) {
    printf("Receiving Resultant Matrix from Server Failed!\n");
    return -1;
  }

  printf("Resultant Matrix: \n");
  k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      printf("%d\t", flat[k++]);
    }
    printf("\n");
  }

  close(socdef);

  return 0;
}

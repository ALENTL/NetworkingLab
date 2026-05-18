#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socdef;
  struct sockaddr_in client, server;
  int clientlen = sizeof(client);
  int r = 2, c = 2, a[100][100], b[100][100], result[100][100], flat[100];

  socdef = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (socdef < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = INADDR_ANY;

  if (bind(socdef, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binded Successfully!\n");

  if (recvfrom(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&client,
               (socklen_t *)&clientlen) < 0) {
    printf("Receiving Matrix A Failed!\n");
  }

  int k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      a[i][j] = flat[k++];
    }
  }

  if (recvfrom(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&client,
               (socklen_t *)&clientlen) < 0) {
    printf("Receiving Matrix B Failed!\n");
  }

  k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      b[i][j] = flat[k++];
    }
  }

  printf("Resultant Matrix: (Server Side Addition)\n");
  k = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      result[i][j] = a[i][j] + b[i][j];
      printf("%d\t", result[i][j]);
      flat[k++] = result[i][j];
    }
    printf("\n");
  }

  if (sendto(socdef, flat, sizeof(flat), 0, (struct sockaddr *)&client,
             clientlen) < 0) {
    printf("Sending Data to Client Failed!\n");
    return -1;
  }

  printf("Result Sent to Client Successfully!\n");
  close(socdef);

  return 0;
}

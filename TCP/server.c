#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd, n_sockfd, readval;
  struct sockaddr_in addr;
  char str[100], temp;
  int addrlen = sizeof(addr);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket creation failed!\n");
    return -1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(8086);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binding Finished!\n");

  if (listen(sockfd, 3) < 0) {
    printf("Client connection terminated!\n");
    return -1;
  }
  printf("Listening!\n");

  if ((n_sockfd = accept(sockfd, (struct sockaddr *)&addr,
                         (socklen_t *)&addrlen)) < 0) {
    printf("Error in Connecting to the Client!\n");
    return -1;
  }
  printf("Client Socket Connection Established!\n");

  readval = read(n_sockfd, str, sizeof(str));

  for (int i = 0, j = strlen(str) - 1; i < j; i++, j--) {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  }

  send(n_sockfd, str, sizeof(str), 0);

  return 0;
}

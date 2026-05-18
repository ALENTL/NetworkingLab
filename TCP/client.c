#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd, readval;
  struct sockaddr_in addr;
  char str[100];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  memset(&addr, '0', sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(8086);

  if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) < 0) {
    printf("Error!\n");
    return -1;
  }

  if ((connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    printf("Error in New Socket Creation!\n");
    return -1;
  }

  printf("Enter the string: ");
  fgets(str, sizeof(str), stdin);

  send(sockfd, str, sizeof(str), 0);
  readval = read(sockfd, str, sizeof(str));
  printf("Reverse: %s\n", str);

  return 0;
}

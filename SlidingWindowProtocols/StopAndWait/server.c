#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd, newsock, frame;
  struct sockaddr_in server;
  int clientlen = sizeof(server);
  char buffer[1024] = {0}, ack[1024] = {0};

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

  while (1) {
    memset(buffer, 0, 1024);
    memset(ack, 0, 1024);

    int valread = read(newsock, buffer, 1024);

    if (valread < 0) {
      printf("Connection closed by client!\n");
      return -1;
    }

    sscanf(buffer, "%d", &frame);
    printf("Server: Received Frame: %d\n", frame);

    snprintf(ack, sizeof(ack), "ACK for frame: %d", frame);
    send(newsock, ack, strlen(ack), 0);
    printf("Server: Sent acknowledgement for frame: %d\n", frame);
  }

  close(newsock);
  close(sockfd);

  return 0;
}

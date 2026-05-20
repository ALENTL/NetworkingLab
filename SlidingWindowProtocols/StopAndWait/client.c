#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd;
  struct sockaddr_in server;
  char buffer[1024] = {0}, ack[1024] = {0};
  int frame = 0, totalFrames;

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

  printf("Enter the total number of frames to send: ");
  scanf("%d", &totalFrames);

  while (frame < totalFrames) {
    snprintf(buffer, sizeof(buffer), "%d", frame);
    send(sockfd, buffer, strlen(buffer), 0);
    printf("Client: Sent Frame: %d\n", frame);

    memset(buffer, 0, 1024);
    int valread = read(sockfd, buffer, 1024);

    if (valread > 0) {
      printf("Client: Received Acknowledgement: %s\n", buffer);
      frame++;
    } else {
      printf("Client: No Acknowledgement Received. Retransmitting Frame: %d\n",
             frame);
    }
  }
  printf("ALl Frames Sent Successfully!\n");
  close(sockfd);

  return 0;
}

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd, connfd;
  struct sockaddr_in server, client;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Binding Failed!\n");
    return -1;
  }
  printf("Binded Successfully!\n");

  if (listen(sockfd, 3) < 0) {
    printf("Listen Failed!\n");
    return -1;
  }
  printf("Listening! ... \n");

  int clientlen = sizeof(client);
  connfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&clientlen);

  if (connfd < 0) {
    printf("Conneiction to Server Failed!\n");
    return -1;
  }
  printf("Connection to the Server Established!\n");

  char command[100] = {0}, sender_mail_addr[100] = {0},
       recipient_mail_addr[100] = {0}, body[200] = {0};

  printf("Enter HELO command to start the communication: ");
  read(connfd, command, sizeof(command));

  if (strncmp(command, "HELO", 4) == 0) {
    printf("HELO command received\nCommunication Established!\n");
  }

  while (1) {
    read(connfd, command, sizeof(command));

    if (strncmp(command, "MAIL FROM", 9) == 0) {
      read(connfd, sender_mail_addr, sizeof(sender_mail_addr));
    } else if (strncmp(command, "RCPT TO", 7) == 0) {
      read(connfd, recipient_mail_addr, sizeof(recipient_mail_addr));
    } else if (strncmp(command, "DATA", 4) == 0) {
      read(connfd, body, sizeof(body));
    } else if (strncmp(command, "QUIT", 4) == 0) {
      exit(0);
    } else {
      printf("Invalid Command\n");
    }

    if (strcmp(sender_mail_addr, "") != 0 &&
        strcmp(recipient_mail_addr, "") != 0 && strcmp(body, "") != 0) {
      printf("\nMail Received!\n");
      printf("Sender Mail Address: %s", sender_mail_addr);
      printf("Recipient Mail Address: %s", recipient_mail_addr);
      printf("Body: %s", body);

      strcpy(sender_mail_addr, "");
      strcpy(recipient_mail_addr, "");
      strcpy(body, "");
    }
  }

  return 0;
}

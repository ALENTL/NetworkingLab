#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8086

int main() {
  int sock_fd, connfd, clientlen;
  struct sockaddr_in server, client;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Socket Binding Failed!\n");
    return -1;
  }
  printf("Socket Binded Successfully!\n");

  if (listen(sock_fd, 3) < 0) {
    printf("Listening Failed!\n");
    return -1;
  }
  printf("Listening...\n");

  connfd = accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&clientlen);

  if (connfd < 0) {
    printf("Server Client Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  char command[100] = "", sender_mail_addr[100] = "", rcpt_mail_addr[100] = "",
       body[200] = "";

  printf("Enter HELO to start communication: ");
  read(connfd, command, sizeof(command));

  if (strncmp(command, "HELO", 4) == 0) {
    printf("HELO command received\nCommunication Established\n");

    while (1) {
      read(connfd, command, sizeof(command));

      if (strncmp(command, "MAIL FROM", 9) == 0) {
        read(connfd, sender_mail_addr, sizeof(sender_mail_addr));
      } else if (strncmp(command, "RCPT TO", 7) == 0) {
        read(connfd, rcpt_mail_addr, sizeof(rcpt_mail_addr));
      } else if (strncmp(command, "DATA", 4) == 0) {
        read(connfd, body, sizeof(body));
      } else if (strncmp(command, "QUIT", 4) == 0) {
        printf("Quitting\n");
        exit(0);
      } else {
        printf("Invalid Command!\n");
      }

      if (strcmp(body, "") != 0 && strcmp(rcpt_mail_addr, "") != 0 &&
          strcmp(sender_mail_addr, "") != 0) {
        printf("\nMail Received.\n");
        printf("Sender Mail Address: %s\n", sender_mail_addr);
        printf("Recipient Mail Address: %s\n", rcpt_mail_addr);
        printf("Body: %s\n", body);
        strcpy(sender_mail_addr, "");
        strcpy(rcpt_mail_addr, "");
        strcpy(body, "");
      }
    }
  }
}

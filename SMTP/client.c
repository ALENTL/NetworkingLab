#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);
  server.sin_addr.s_addr = INADDR_ANY;

  if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connection Failed!\n");
    return -1;
  }
  printf("Connection Established!\n");

  char command[100] = {0}, sender_mail_addr[100] = {0},
       recipient_mail_addr[100] = {0}, body[200] = {0};

  printf("Enter HELO command to start communication with server: ");
  fgets(command, sizeof(command), stdin);
  write(sockfd, command, sizeof(command));

  if (strncmp(command, "HELO", 4) == 0) {
    printf("HELO command sent\nCommunication Established!\n");

    while (1) {
      printf("\n1. MAIL FROM: Define sender mail address\n");
      printf("2. RCPT TO: Define receivers mail address\n");
      printf("3. DATA: Define body of the mail\n");
      printf("4. QUIT\n");

      printf("Enter your selection: ");
      fgets(command, sizeof(command), stdin);
      write(sockfd, command, sizeof(command));

      if (strncmp(command, "MAIL FROM", 9) == 0) {
        printf("Enter sender's mail address: ");
        fgets(sender_mail_addr, sizeof(sender_mail_addr), stdin);
        write(sockfd, sender_mail_addr, sizeof(sender_mail_addr));
      } else if (strncmp(command, "RCPT TO", 7) == 0) {
        printf("Enter receivers mail address: ");
        fgets(recipient_mail_addr, sizeof(recipient_mail_addr), stdin);
        write(sockfd, recipient_mail_addr, sizeof(recipient_mail_addr));
      } else if (strncmp(command, "DATA", 4) == 0) {
        printf("Enter mail body: ");
        fgets(body, sizeof(body), stdin);
        write(sockfd, body, sizeof(body));
      } else if (strncmp(command, "QUIT", 4) == 0) {
        exit(0);
      } else {
        printf("Invalid Command!\n");
      }

      if (strcmp(sender_mail_addr, "") != 0 &&
          strcmp(recipient_mail_addr, "") != 0 && strcmp(body, "") != 0) {
        printf("\nMail Sent!\n");
        printf("Sender Mail Address: %s", sender_mail_addr);
        printf("Recipient Mail Address: %s", recipient_mail_addr);
        printf("Body: %s", body);

        strcpy(sender_mail_addr, "");
        strcpy(recipient_mail_addr, "");
        strcpy(body, "");
      }
    }
  }
}

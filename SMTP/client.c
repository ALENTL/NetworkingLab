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
  int sock_fd;
  struct sockaddr_in server;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd < 0) {
    printf("Socket Creation Failed!\n");
    return -1;
  }
  printf("Socket Created Successfully!\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(8086);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 0) {
    printf("Error!\n");
    return -1;
  }

  if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Client-Server Connection Failed!\n");
    return -1;
  }
  printf("Client-Server Connection Established!\n");

  char command[100] = "", sender_mail_addr[100] = "", rcpt_mail_addr[100] = "",
       body[200] = "";

  printf("Enter HELO command to start communicating with the server: ");
  fgets(command, sizeof(command), stdin);
  write(sock_fd, command, sizeof(command));

  if (strncmp(command, "HELO", 4) == 0) {
    printf("HELO command sent\n Communication Established!\n");

    while (1) {
      printf("\n1. MAIL FROM - Define sender mail address\n");
      printf("\n2. RCPT TO - Define receiver mail address\n");
      printf("\n3. DATA - Define body of the mail\n");
      printf("\n4. QUIT\n");

      printf("Enter a command: ");
      fgets(command, sizeof(command), stdin);
      write(sock_fd, command, sizeof(command));

      if (strncmp(command, "MAIL FROM", 9) == 0) {
        printf("Enter sender's mail: ");
        fgets(sender_mail_addr, sizeof(sender_mail_addr), stdin);
        write(sock_fd, sender_mail_addr, sizeof(sender_mail_addr));
      } else if (strncmp(command, "RCPT TO", 7) == 0) {
        printf("Enter recipient mail address: ");
        fgets(rcpt_mail_addr, sizeof(rcpt_mail_addr), stdin);
        write(sock_fd, rcpt_mail_addr, sizeof(rcpt_mail_addr));
      } else if (strncmp(command, "DATA", 4) == 0) {
        printf("Enter mail body: ");
        fgets(body, sizeof(body), stdin);
        write(sock_fd, body, sizeof(body));
      } else if (strncmp(command, "QUIT", 4) == 0) {
        write(sock_fd, command, sizeof(command));
        exit(0);
      } else {
        printf("Invalid Command!\n ");
      }

      if (strcmp(sender_mail_addr, "") != 0 &&
          strcmp(rcpt_mail_addr, "") != 0 && strcmp(body, "") != 0) {
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

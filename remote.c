#include <stdio.h>
#include "remote.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main () {
  int choice = 0;
  char hostip[15];
  char hostport[5];

  printf("Enter the ip address of the computer you wish to control.\n");
  scanf("%s", hostip);
  printf("You entered: %s\n", hostip);
  
  printf("Enter the port of the program on computer you wish to control.\n");
  scanf("%s", hostport);
  printf("You entered: %s\n", hostport);

  //TODO Fill in hints
  
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(hostip, hostport, &hints, &res);
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  
  int len = 0;
  int bytes_sent = 0;
  char *msg;
  while (choice != 4){
    printf("Choose a number 1-play/pause 2-forward 3-back 4-stop\n");
    scanf("%d", &choice);
    switch(choice){
    case 1:
      msg = "1";
      len = strlen(msg);
      bytes_sent = send(sockfd, msg, len, 0);
      printf("play/pause\n");
      break;
    case 2:
      msg = "2";
      len = strlen(msg);
      bytes_sent = send(sockfd, msg, len, 0);
      printf("forward\n");
      break;
    case 3:
      msg = "3";
      len = strlen(msg);
      bytes_sent = send(sockfd, msg, len, 0);
      printf("back\n");
      break;
    case 4:
      printf("stop\n");
      break;
    }
  }
  close(sockfd);
  return 0;
}

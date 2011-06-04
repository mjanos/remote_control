#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  else {
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
  }
}

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
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  
  struct addrinfo *servinfo;
  int returnval =  getaddrinfo(hostip, hostport, &hints, &servinfo);
  if (returnval != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnval));
    return 1;
  }

  //loop through all results
  int sockfd; 
  struct addrinfo *p;
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, 
                         p->ai_socktype, 
                         p->ai_protocol)) == -1) {
      perror("client: socket"); //prints error
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break; //on success quit the loop
  }
  if (p == NULL) { //if nothing connects
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  
  char s[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, 
            sizeof s);
  printf("client: connecting to %s\n", s);

  freeaddrinfo(servinfo);

  //actual program
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
      if (send(sockfd, msg, len, 0) == -1) {
        perror("send");
      }
      printf("play/pause\n");
      break;
    case 2:
      msg = "2";
      len = strlen(msg);
      if (send(sockfd, msg, len, 0) == -1) {
        perror("send");
      }
      printf("forward\n");
      break;
    case 3:
      msg = "3";
      len = strlen(msg);
      if (send(sockfd, msg, len, 0) == -1) {
        perror("send");
      }
      printf("back\n");
      break;
    case 4:
      msg = "4";
      len = strlen(msg);
      if (send(sockfd, msg, len, 0) == -1) {
        perror("send");
      }
      printf("stop\n");
      break;
    }
  }
  close(sockfd);
  return 0;
}

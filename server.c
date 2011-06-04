#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>

#define BACKLOG 10 //num connections the queue will hold (?)
#define MAXDATASIZE 100 //max bytes per recv

void* get_in_addr (struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  else {
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
  }
}

int main (int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: server PORT_NUMBER");
    return 1;
  }
  char *PORT = argv[1];
  printf("YOU ENTERED PORT: %s\n", PORT);

  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 2;
  }
  
  int sockfd, yes;
  yes = 1;
  struct addrinfo *curaddr;
  for (curaddr = servinfo; curaddr != NULL; curaddr = curaddr->ai_next) {
    if ((sockfd = socket(curaddr->ai_family,
                         curaddr->ai_socktype,
                         curaddr->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
      perror("server: setsockopt");
      continue;
      //exit(1);
    }
    if (bind(sockfd, curaddr->ai_addr, curaddr->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }
  if (curaddr == NULL) {
    fprintf(stderr, "server: failed to create and bind socket\n");
    return 3;
  }
  freeaddrinfo(servinfo);
  
  if (listen(sockfd, BACKLOG) == -1) {
    perror("server: listen");
    exit(1);
  }
  
  printf("server: waiting for connections...\n");

  struct sockaddr_storage their_addr; //connectors address
  socklen_t sin_size = sizeof their_addr; //32 bit int
  int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
  if (new_fd == -1) {
    perror("accept");
    exit(1);
  }
  close (sockfd);

  char s[INET6_ADDRSTRLEN];
  inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
  printf("server: got connection from %s\n", s);
  
  char buf[MAXDATASIZE];
  int numbytes;
  while (buf[0] != '4') {
    memset(&buf, 0, sizeof buf);
    numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0);//-1 for null char
    if (numbytes == -1) {
      perror("recv");
      exit(1);
    }
    buf[numbytes] = '\0';//null char
    printf("server: received '%s'\n", buf);
    switch (buf[0]) {
    case '1' : printf("play/pause\n");
      break;
    case '2' : printf("forward\n");
      break;
    case '3' : printf("back\n");
      break;
    case '4' : printf("stop\n");
      break;
    }
  }
  close(new_fd);
  return 0;
}

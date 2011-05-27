#include "remote.h"


int main(){
  int choice = 0;
  char hostip[15];
  printf("Please enter the ip address of the computer you wish to control.\n");
  scanf("%s", hostip);
  printf("You entered: %s\n", hostip);
  while (choice != 4){
    printf("Choose a number 1-play/pause 2-forward 3-back 4-stop\n");
    scanf("%d", &choice);
    switch(choice){
    case 1:
      printf("play/pause\n");
      break;
    case 2:
      printf("forward\n");
      break;
    case 3:
      printf("back\n");
      break;
    case 4:
      printf("stop\n");
      break;
    }
  }
  return 0;
}

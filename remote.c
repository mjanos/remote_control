#include "remote.h"


int main(){
  int choice;
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
  return 0;
}

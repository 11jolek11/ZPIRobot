#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "libserial.h"
#include "rover.h"

int main(int argc, char *argv[])
{
  init_serial("/dev/serial0");
  //printf("What %s\n", argv[2]);
  char buffer[100];
  for (int i=1; i < argc; ++i){
    strcpy(buffer, argv[i]);
    send(move(atof(buffer)));
    
    usleep(500000);
  }
  send(move(0.0f));
  serial_close_connection();
  //int value = stoi(argv[1]);
  //send(move(value));
  return 0;
}

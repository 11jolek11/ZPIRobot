#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "cJSON.h"
#include "libserial.h"
#include "rover.h"
#include "safe_ring_buffer.h"

// speed 0.1 <-> 0.5
float SPEED = 0.1f;
float speed_delta = 0.1f;

int main(int argc, char *argv[])
{
  init_serial("/dev/serial0");
  //printf("What %s\n", argv[2]);
  char buffer[100];
  ring_buffer command = create_ring_buffer(32);
  usleep(500000);
  }


  send(move(0.0f));
  serial_close_connection();
  //int value = stoi(argv[1]);
  //send(move(value));
  return 0;
}

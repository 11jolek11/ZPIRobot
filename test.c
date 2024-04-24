#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "cJSON.h"
#include "libserial.h"
#include "rover.h"
#include "decoder.h"

// MINUS 1ffc03e
// PLUS 1ff50ae

// speed 0.1 <-> 0.5
float SPEED = 0.1f;
float speed_delta = 0.1f;

enum BUTTONS {
  LEFT = 0x1fe21de,
  RIGHT = 0x1feb54a,
  FORWARD = 0x1fe31ce,
  BACKWARD = 0x1fe956a,
  SPEED_UP = 0x1ff50ae,
  SLOW_DOWN = 0x1ffc03e
};

float slower(float speed)
{
  if (speed >= 0.0f + speed_delta)
  {
    speed -= speed_delta;
  }
  return speed;
}

float faster(float speed)
{
  if (speed <= 0.5f - speed_delta)
  {
    speed += speed_delta;
  }
  return speed;  
}

int main(int argc, char *argv[])
{
  init_serial("/dev/serial0");
  //printf("What %s\n", argv[2]);
  char buffer[100];
  struct ring_buffer command = create_ring_buffer(0);
  int direction = 1; // 1 - forward -1 - back
  int left_right = 0; // 1 - left -1 - right

  for (int i=1; i < argc; ++i){

    int counter = 10000;
    while (counter) {
      counter--;
    //   ring_buffer_put(command, get_raw());

        ring_buffer_put(command, getchar());
    }
      uint32_t single_command = ring_buffer_get(command);
      switch (single_command) {
        case FORWARD:
            send(move(SPEED));
            direction = 1;
            break;
        case BACKWARD:
            send(move((-1)*SPEED));
            direction = -1;
            break;
        case LEFT:
            left_right = 1;
            if (direction == 1) {
                send(move_sides(1, SPEED - 0.1f, 1, SPEED + 0.1f));
            } else {
                send(move_sides(1, (-1)*(SPEED - 0.1f), 1, (-1)*(SPEED + 0.1f)));
            }
            break;
        case RIGHT:
            left_right = -1;
            if (direction == 1) {
                send(move_sides(1, SPEED + 0.1f, 1, SPEED - 0.1f));
            } else {
                send(move_sides(1, (-1)*(SPEED + 0.1f), 1, (-1)*(SPEED - 0.1f)));
            }
            // send(move_sides(1, SPEED + 0.1f, 1, SPEED - 0.1f));
            break;
        case SPEED_UP:
            SPEED = faster(SPEED);
            break;
        case SLOW_DOWN:
            SPEED = slower(SPEED);
            break;
    }

    // strcpy(buffer, argv[i]);
    // send(move(atof(buffer)));

    usleep(500000);
  }


  send(move(0.0f));
  serial_close_connection();
  //int value = stoi(argv[1]);
  //send(move(value));
  return 0;
}

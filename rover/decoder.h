#include <stdio.h>
#include <stdint.h>
#include <pigpio.h>
#include <signal.h>

void sigint_handler(void);
int get_raw(int input_pin);

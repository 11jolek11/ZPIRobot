#include "./decoder.h"

static int running = 1;

void sigint_handler(void) {
        running = 0;
        gpioTerminate();
}

int get_raw(int input_pin) {
        printf("\nNew mes\n");
        uint32_t command = 0x0;
        int num1s = 0;

        int counter = 0;

        //struct pulse command[66];

        int prev_value = 0;
        int value = gpioRead(input_pin);

        while(value) {
                //gpioSleep(0, 0, 1);
                value = gpioRead(input_pin);
        }

        uint32_t start_time = gpioTick();

        while(num1s <= 100000) {
                if (value != prev_value) {
                        uint32_t now = gpioTick();
                        uint32_t pulse_length = now - start_time;
                        start_time = now;
                        //printf("%d. State: %d # %f\n", counter,  value,  pulse_length/1000.0);

                        if ((!value) && (pulse_length > 1000)) {
                                printf("1");
                                command = (command+1) << 1;
                        }

                        if ((!value) && (pulse_length < 1000)) {
                                printf("0");
                                command = command << 1;
                        }
                        counter++;
                        // command <-- append prev_value and pulse_length
                }

                if (value) {
                        num1s++;
                } else {
                        num1s = 0;
                }

                prev_value = value;
                value = gpioRead(input_pin);
        }
        return command;
}


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "cJSON.h"
#include "libserial.h"


void stoi(int integer, char* buffer)
{
    snprintf(buffer, sizeof(buffer)-1, "%d", integer);
}

void left_motor(cJSON* json, int speed)
{
    if (speed > 255) {
        speed = 255;
    }
    else if (speed < -255) {
        speed = -255;
    }

    cJSON* forT = cJSON_CreateNumber(speed);
    cJSON_AddItemToObject(json, "L", forT);
}

void right_motor(cJSON* json, int speed)
{
    if (speed > 255) {
        speed = 255;
    }
    else if (speed < -255) {
        speed = -255;
    }

    cJSON* forT = cJSON_CreateNumber(speed);
    cJSON_AddItemToObject(json, "R", forT);

}

char* move(int speed)
{
    cJSON* json = cJSON_CreateObject();
    cJSON* forT = cJSON_CreateNumber(11);

    cJSON_AddItemToObject(json, "T", forT);

    if (speed > 255) {
        speed = 255;
    }
    else if (speed < -255) {
        speed = -255;
    }

    left_motor(json, speed);
    right_motor(json, speed);

    char* json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char* stop()
{
    cJSON* json = cJSON_CreateObject();
    cJSON* forT = cJSON_CreateNumber(0);
    cJSON_AddItemToObject(json, "T", forT);

    char* json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char* move_sides(bool left, int left_speed, bool right, int right_speed)
{
    cJSON* json = cJSON_CreateObject();
    cJSON* forT = cJSON_CreateNumber(11);
    cJSON_AddItemToObject(json, "T", forT);

    if (left == 1){
        left_motor(json, left_speed);

    }
    if (right == 1){
        right_motor(json, right_speed);
    }

    char* json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char* status()
{
    cJSON* json = cJSON_CreateObject();
    cJSON* forT = cJSON_CreateNumber(126);
    cJSON_AddItemToObject(json, "T", forT);

    char* json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

int main()
{
    move_sides(1, 10, 1, 15);
    move(10);
    stop();
    status();
    init_serial("/dev/serial0");
    char buffer[100];
    char request[100];
    strcpy(request, move(10));
    serial_send_and_receive(request, buffer);
    cJSON* json = cJSON_Parse(buffer);
    char* output = cJSON_Print(json);
    printf("%s", output);
    return 0;
}

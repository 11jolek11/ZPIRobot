#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "libserial.h"


void stoi(int integer, char *buffer)
{
    snprintf(buffer, sizeof(buffer)-1, "%d", integer);
}

void left_motor(cJSON *json, float speed)
{
    if (speed > 0.5) {
        speed = 0.5;
    }
    else if (speed < -0.5) {
        speed = -0.5;
    }

    cJSON *forT = cJSON_CreateNumber(speed);
    cJSON_AddItemToObject(json, "L", forT);
}

void right_motor(cJSON *json, float speed)
{
    if (speed > 0.5) {
        speed = 0.5;
    }
    else if (speed < -0.5) {
        speed = -0.5;
    }

    cJSON *forT = cJSON_CreateNumber(speed);
    cJSON_AddItemToObject(json, "R", forT);

}

char *move(float speed)
{
    cJSON *json = cJSON_CreateObject();
    cJSON *forT = cJSON_CreateNumber(1);

    cJSON_AddItemToObject(json, "T", forT);

    left_motor(json, speed);
    right_motor(json, speed);

    char *json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    
    return json_string;
}

char *move_int(int speed_int)
{
    cJSON *json = cJSON_CreateObject();
    cJSON *forT = cJSON_CreateNumber(1);

    cJSON_AddItemToObject(json, "T", forT);
    
    float speed = (float)speed_int/200;

    left_motor(json, speed);
    right_motor(json, speed);

    char *json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    
    return json_string;
}

char *stop()
{
    cJSON *json = cJSON_CreateObject();
    cJSON *forT = cJSON_CreateNumber(0);
    cJSON_AddItemToObject(json, "T", forT);

    char *json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char *move_sides(bool left, float left_speed, bool right, float right_speed)
{
    cJSON *json = cJSON_CreateObject();
    cJSON *forT = cJSON_CreateNumber(1);
    cJSON_AddItemToObject(json, "T", forT);

    if (left == 1){
        left_motor(json, left_speed);

    }
    if (right == 1){
        right_motor(json, right_speed);
    }

    char *json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char *status()
{
    cJSON *json = cJSON_CreateObject();
    cJSON *forT = cJSON_CreateNumber(126);
    cJSON_AddItemToObject(json, "T", forT);

    char *json_string = cJSON_PrintUnformatted(json);
    printf("Sent json to serial: %s \n", json_string);
    cJSON_Delete(json);
    return json_string;
}

char *send(char *command)
{
  char buffer[255];
  char request[255];
  strcpy(request, command);
  serial_send_and_receive(request, buffer);
  cJSON *json = cJSON_Parse(buffer);
  char *output = cJSON_PrintUnformatted(json);
  cJSON_Delete(json);
  return output;
}


#ifndef rover_H
#define rover_H

void stoi(int, char *);

void left_motor(cJSON *, float);


void right_motor(cJSON *, float);

char *move(float);

char *move_int(int);

char *stop();

char *move_sides(bool, float, bool, float);

char *status();

char *send(char *);

#endif // rover_H

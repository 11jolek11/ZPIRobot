#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_BUFF_SIZE 1024

//struct request {
//      char[MAX_BUFF_SIZE] command;
//      char[MAX_BUFF_SIZE] data;
//};
//
//struct response {
//      char[MAX_BUFF_SIZE] ;
//      char[MAX_BUFF_SIZE] data;
//}

void write_error_message(char *buffer);
void write_int_value_message(char *buffer,int value);
void write_double_value_message(char *buffer, double value);
void write_double_value_message(char *buffer, double value);


#include "message.h"
//struct request {
//	char[MAX_BUFF_SIZE] command;
//	char[MAX_BUFF_SIZE] data;
//};
//
//struct response {
//	char[MAX_BUFF_SIZE] ;
//	char[MAX_BUFF_SIZE] data;
//}

void write_error_message(char *buffer) {
	cJSON *cjson = cJSON_CreateObject();
	cJSON_AddStringToObject(cjson, "error", "Error");

	char *json = cJSON_Print(cjson);
	strcpy(buffer, json);

	cJSON_free(json);
	cJSON_Delete(cjson);
}

void write_int_value_message(char *buffer,int value) {
	cJSON *cjson = cJSON_CreateObject();
	cJSON_AddStringToObject(cjson, "status", "ok");
	cJSON_AddNumberToObject(cjson, "value", (double)value);

	char *json = cJSON_Print(cjson);
	strcpy(buffer, json);
	cJSON_free(json);
	cJSON_Delete(cjson);
}

void write_double_value_message(char *buffer, double value) {
	cJSON *cjson = cJSON_CreateObject();
	cJSON_AddStringToObject(cjson, "status", "ok");
	cJSON_AddNumberToObject(cjson, "value", value);

	char *json = cJSON_Print(cjson);
	strcpy(buffer, json);
	cJSON_free(json);
	cJSON_Delete(cjson);
}

//int main() {
//	char buffer[1024];
//	memset(buffer, 0, 1024);
//
//	write_error_message(buffer);
//	printf("%s \n", buffer);
//}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <cjson/cJSON.h>
#include <pthread.h>

#include "rssi.h"
#include "callback.h"
#include "message.h"

#define BUFFER_SIZE 1024

static hackrf_device* device = NULL;
static sdr_module settings;


//static const int samplestoaverage = 1024;

int verbose = 1;
float numSeconds = 5.0;
uint64_t frequency = 88600000;
uint32_t lnagain = 32;
uint32_t vgagain = 44;
double bandwidth = 1000000;

int doneReceiving = 0;

static const char* socket_path = "/tmp/mysocket";
static const unsigned int backlog_size = 5;

int init_sdr() {
	set_default_settings(&settings);
	settings.device = device;
	settings.rx_callback = rx_callback;

    int result;

	// libhackrf must be initialized first
	result = hackrf_init();
	if( result != HACKRF_SUCCESS ) {
	printf("hackrf_init() failed: %s (%d)\n", hackrf_error_name(result), result);

	return EXIT_FAILURE;
	}

	// attempt to open first HackRF device seen
	result = hackrf_open(&device);
	if( result != HACKRF_SUCCESS ) {
	printf("hackrf_open() failed: %s (%d)\n", hackrf_error_name(result), result);
	
	return EXIT_FAILURE;
	}

	// set freq gain and bandwidth
	if(verbose) printf("Setting frequency to %llu Hz...", frequency);
	result = hackrf_set_freq(device, frequency);
	if( result != HACKRF_SUCCESS ) {
	printf("hackrf_set_freq() failed: %s (%d)\n", hackrf_error_name(result), result);
	return EXIT_FAILURE;
	}
	result = hackrf_set_lna_gain(device, lnagain);
	result |= hackrf_set_vga_gain(device, vgagain);
	if( result != HACKRF_SUCCESS ) {
	printf("hackrf_set_gains failed: %s (%d)\n", hackrf_error_name(result), result);
	return EXIT_FAILURE;
	}
	result = hackrf_set_baseband_filter_bandwidth(device, hackrf_compute_baseband_filter_bw(bandwidth));
	result = hackrf_set_sample_rate(device, bandwidth);
	if( result != HACKRF_SUCCESS ) {
	printf("Setting bandwidth failed: %s (%d)\n", hackrf_error_name(result), result);
	return EXIT_FAILURE;
	}
	return 0;
}

void handle_connection(int connection) {
        int data_recv = 0;
        char recv_buf[BUFFER_SIZE];
        char send_buf[BUFFER_SIZE];
        do {
            memset(recv_buf, 0, (BUFFER_SIZE)*sizeof(char));
            memset(send_buf, 0, (BUFFER_SIZE)*sizeof(char));
            data_recv = recv(connection, recv_buf, BUFFER_SIZE, 0);

            if (data_recv > 0) {
		//recv_buf[BUFFER_SIZE-1] = 0;

                printf("Data received: %d : %s \n", data_recv, recv_buf);

		cJSON *json = cJSON_Parse(recv_buf);

		if (json == NULL) {
		    const char *error_ptr = cJSON_GetErrorPtr();
		    if (error_ptr != NULL) {
		        printf("Error: %s \n", error_ptr);
		    }
		    return;
		}

		cJSON *command = cJSON_GetObjectItemCaseSensitive(json, "command");
		cJSON *value =  cJSON_GetObjectItemCaseSensitive(json, "value");

		if (cJSON_IsString(command) && (command->valuestring != NULL)) {
		    printf("Command: %s ", command->valuestring);
		    if (strcmp(command->valuestring, "get_rssi") == 0) {
		    	printf("is rssi request %g \n", get_current_rssi());
		    } else if (strcmp(command->valuestring, "set_vga") == 0) {	
			printf("New vga value arrived %f \n", value->valuedouble);
			if (cJSON_IsNumber(value) && (value->valuedouble >= 0)) {
		            printf("set vga\n");
			    int *temp = (int *)&value->valuedouble;
			    printf("HIT");
			    int result = set_new_vga_gain(&settings, *temp);
			    write_int_value_message(send_buf, result);
			} else {
				printf("ERROR in set_vga \n");
				write_error_message(send_buf);	
			}
		    } else if (strcmp(command->valuestring, "set_lga") == 0) {
		        // do stuff ...
		    	printf("set lga\n");
		    }  else if (strcmp(command->valuestring, "set_bandwith") == 0) {
		        // do stuff ...
		    	printf("set band\n");
		    } else if (strcmp(command->valuestring, "set_frequency") == 0) {
		        // do stuff ...
		    	printf("get freq\n");
		    } else if (strcmp(command->valuestring, "get_vga") == 0) {
		        // do stuff ...
		    	printf("get vga\n");
		    } else if (strcmp(command->valuestring, "get_lga") == 0) {
		        // do stuff ...
		    	printf("get lga\n");
		    }  else if (strcmp(command->valuestring, "get_bandwith") == 0) {
		        // do stuff ...
		    	printf("get band\n");
		    } else if (strcmp(command->valuestring, "get_frequency") == 0) {
		        // do stuff ...
		    	printf("get freq\n");
		    } else {
		        // response with bad request
		    	printf("404\n");
		    }

		    printf("\n");
		}

		// TODO(11jolek11): fix 
               //strcpy(send_buf, "Got message: ");
               //strcat(send_buf, recv_buf);


		//printf("Rssi: %f \n", get_current_rssi());

		printf("Trying transfer: %zu: %s\n", strlen(send_buf), send_buf);
                if (send(connection, send_buf, strlen(send_buf)*sizeof(char), 0) == -1) {
                    printf("Error on send() call \n");
                }
            } else {
                printf(">> Error on recv \n");
            }
        } while (data_recv > 0);
}

int main() {
    // create server side
    int socket_fd = 0;
    int connection = 0;
    int verbose = 1;
    struct sockaddr_un local, remote;

    printf("SDR: %d\n", init_sdr());

    int addr_len = 0;
    int result;

	 // start receiving, call rx_callback() when data received
	  if(verbose) printf("Starting reception...\r\n");
	  result = hackrf_start_rx(device, rx_callback, NULL);
	  if( result != HACKRF_SUCCESS ) {
	    printf("hackrf_start_rx() failed: %s (%d)\n", hackrf_error_name(result), result);
	    return EXIT_FAILURE;
	  } else {
	    if(verbose) printf("hackrf_start_rx() done.\r\n");
	  }

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( socket_fd == -1 ) {
        printf(">> Error on socket creation \n");
        return 1;
    }

    local.sun_family = AF_UNIX;
    strcpy( local.sun_path, socket_path );
    unlink(local.sun_path);
    addr_len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(socket_fd, (struct sockaddr*)&local, addr_len) != 0) {
        printf(">> Error on binding socket \n");
        return 1;
    }

    if ( listen(socket_fd, backlog_size) != 0 ) {
        printf(">> Error on listen call \n");
    }

    int8_t server_running = 1;

    printf(">> Waiting for connection.... \n");
    while (server_running) {
        unsigned int sock_len = 0;
        if( (connection = accept(socket_fd, (struct sockaddr*)&remote, &sock_len)) == -1 ) {
            printf(">> Error on accept \n");
            return 1;
        }

        printf(">> Server connected \n");

        handle_connection(connection);
    }
    close(connection);
    close(socket_fd);

  // stop receiving
  result = hackrf_stop_rx(device);
  if( result != HACKRF_SUCCESS ) {
    printf("hackrf_stop_rx() failed: %s (%d)\n", hackrf_error_name(result), result);
  }else {
    if(verbose) printf("hackrf_stop_rx() done\n");
  }

  // close device
  result = hackrf_close(device);
  pthread_mutex_unlock(&settings.lock);
  pthread_mutex_destroy(&settings.lock);
  if( result != HACKRF_SUCCESS )
  {
    printf("hackrf_close() failed: %s (%d)\n", hackrf_error_name(result), result);
  }else {
    if(verbose) printf("hackrf_close() done\n");
  }

  hackrf_exit();
  if(verbose) printf("hackrf_exit() done\n");

    return 0;
}


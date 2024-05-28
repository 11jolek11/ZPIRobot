#include <libhackrf/hackrf.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <liquid/liquid.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

//#include "settings.h"
//#include "rssi.h"

#define SAMPLE_SIZE 1024
#define CALLBACK_VERBOSE 0

typedef struct {
        hackrf_device *device;
        uint32_t vga_gain;
        uint32_t lna_gain;
        uint64_t frequency;
        double bandwidth;
        hackrf_sample_block_cb_fn rx_callback;
        pthread_mutex_t lock;
} sdr_module;

void set_verbose_output();
float* get_rssi_p();
float get_current_rssi();
int rx_callback(hackrf_transfer* transfer);
int init_sdr();
int start_rssi_capture();
int stop_rssi_capture();
int cleanup_sdr();
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

void set_default_settings();
int set_new_vga_gain(uint32_t new_vga);
int set_new_lna_gain(uint32_t new_lna);
int set_new_bandwith(double new_bandwidth);
int set_new_frequency(uint64_t new_frequency);
// ######################################################################################################################################
uint32_t get_new_vga_gain();
uint32_t get_new_lna_gain();
double get_new_bandwith();
uint64_t get_new_frequency();

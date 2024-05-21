#include <libhackrf/hackrf.h>
#include <pthread.h>
#include <stdint.h>
#include "rssi.h"

typedef struct device_settings {
        hackrf_device *device;
        uint32_t vga_gain;
        uint32_t lna_gain;
        uint64_t frequency;
        double bandwidth;
        hackrf_sample_block_cb_fn rx_callback;
	pthread_mutex_t lock;
} sdr_module;

void set_default_settings(sdr_module *sdr); 
int set_new_vga_gain(sdr_module *sdr, uint32_t new_vga);

int set_new_lna_gain(sdr_module *sdr, uint32_t new_lna);
int set_new_bandwidth(sdr_module *sdr, double new_bandwidth);

int set_new_frequency(sdr_module *sdr, uint64_t new_frequency);


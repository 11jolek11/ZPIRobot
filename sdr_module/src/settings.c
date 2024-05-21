//#include <libhackrf/hackrf.h>
//#include <stdint.h>
//#include <rssi.h>
//#include <pthreads.h>
#include "settings.h"
//typedef struct device_settings {
//	hackrf_device *device;
//	uint32_t vga_gain;
//	uint32_t lna_gain;
//	uint64_t frequency;
//	double bandwith;
//	hackrf_sample_block_cb_fn rx_callback;
//	pthread_mutex_t lock;
//} sdr_module;

//int set_new_gains(sdr_module *sdr, uint32_t new_vga, uint32_t new_lna) {
//   int device_status_code = hackrf_stop_rx(sdr->device);
//   int result = hackrf_set_lna_gain(sdr->device, new_lna) | hackrf_set_vga_gain(sdr->device, new_vga);
//   int start_status_code = hackrf_start_rx(sdr->device, sdr->rx_callback, NULL);
//
//   if (!result) {
//       sdr->vga_gain = new_vga;
//   }
//       
//   return result | (device_status_code << 1) | (start_status_code << 2);
//

//uint64_t frequency = ;
//uint32_t lnagain = ;
//uint32_t vgagain = ;
//double bandwidth = ;

void set_default_settings(sdr_module *sdr) {
	sdr->frequency = 88600000;
	sdr->lna_gain = 32;
	sdr->vga_gain = 44;
	sdr->bandwidth = 1000000;
	//sdr->lock = PTHREAD_MUTEX_INITIALIZER;

	if (pthread_mutex_init(&sdr->lock, NULL) != 0) {
	    printf("\n mutex init has failed\n");
	} 
}

int set_new_vga_gain(sdr_module *sdr, uint32_t new_vga) {
    pthread_mutex_lock(&sdr->lock);
    int device_status_code = hackrf_stop_rx(sdr->device);
    int result = hackrf_set_vga_gain(sdr->device, new_vga);
    int start_status_code = hackrf_start_rx(sdr->device, sdr->rx_callback, NULL);

    if (!result) {
    	sdr->vga_gain = new_vga;
    }
	
    pthread_mutex_unlock(&sdr->lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_lna_gain(sdr_module *sdr, uint32_t new_lna) {
    pthread_mutex_lock(&sdr->lock);
    int device_status_code = hackrf_stop_rx(sdr->device);
    int result = hackrf_set_lna_gain(sdr->device, new_lna);
    int start_status_code = hackrf_start_rx(sdr->device, sdr->rx_callback, NULL);

    if (!result) {
    	sdr->lna_gain = new_lna;
    }
	
    pthread_mutex_unlock(&sdr->lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_bandwith(sdr_module *sdr, double new_bandwidth) {
    pthread_mutex_lock(&sdr->lock);
    int device_status_code = hackrf_stop_rx(sdr->device);
    int result =  hackrf_set_baseband_filter_bandwidth(sdr->device, hackrf_compute_baseband_filter_bw(new_bandwidth));
    int start_status_code = hackrf_start_rx(sdr->device, sdr->rx_callback, NULL);

    if (!result) {
    	sdr->bandwidth = new_bandwidth;
    }
	
    pthread_mutex_unlock(&sdr->lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_frequency(sdr_module *sdr, uint64_t new_frequency) {
    pthread_mutex_lock(&sdr->lock);
    int device_status_code = hackrf_stop_rx(sdr->device);
    int result = hackrf_set_freq(sdr->device, new_frequency);
    int start_status_code = hackrf_start_rx(sdr->device, sdr->rx_callback, NULL);

    if (!result) {
    	sdr->frequency = new_frequency;
    }
	
    pthread_mutex_unlock(&sdr->lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}
// ######################################################################################################################################
uint32_t get_new_vga_gain(sdr_module *sdr) {
    pthread_mutex_lock(&sdr->lock);	
    uint32_t vga_gain = sdr->vga_gain;
    pthread_mutex_unlock(&sdr->lock);
    return vga_gain;
}

uint32_t get_new_lna_gain(sdr_module *sdr) {
    pthread_mutex_lock(&sdr->lock);
    uint32_t lna_gain = sdr->lna_gain;
    pthread_mutex_unlock(&sdr->lock);
    return lna_gain;
}

double get_new_bandwith(sdr_module *sdr) {
    pthread_mutex_lock(&sdr->lock);
    double bandwidth = sdr->bandwidth;
    pthread_mutex_unlock(&sdr->lock);
    return bandwidth;
}

uint64_t get_new_frequency(sdr_module *sdr) {
    pthread_mutex_lock(&sdr->lock);
    uint64_t freq = sdr->frequency;
    pthread_mutex_unlock(&sdr->lock);
    return freq;
}


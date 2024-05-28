
#include "rssi.h"
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
#include <libhackrf/hackrf.h>
#include <pthread.h>
//static hackrf_device* device = NULL;

static const int samplestoaverage = 1024;

//int verbose = 1;
//float numSeconds = 5.0;
//uint64_t frequency = 88600000;
//uint32_t lnagain = 32;
//uint32_t vgagain = 44;
//double bandwidth = 1000000;

//int doneReceiving = 0;
//static const int samplestoaverage = 1024;

static float rssi;
pthread_mutex_t rssi_lock = PTHREAD_MUTEX_INITIALIZER;

float* get_rssi() {
    return &rssi;
}

float get_current_rssi() {
    pthread_mutex_lock(&rssi_lock);
    float temp_rssi = rssi;
    pthread_mutex_unlock(&rssi_lock);
    return temp_rssi;
}

int rx_callback(hackrf_transfer* transfer) {
  // rx_callback() called every time new data arrives from HackRF

  // Time domain samples go in samples[], output FFT will be in fft[]
  float complex raw_samples[SAMPLE_SIZE];
  float complex samples[SAMPLE_SIZE];
  float complex agc_output;

  // Create IIR filter to remove DC offset
  float alpha = 0.10f;
  iirfilt_crcf dcblock = iirfilt_crcf_create_dc_blocker(alpha);

  // Setup automatic gain control
  agc_crcf agc_rx = agc_crcf_create();
  agc_crcf_set_bandwidth(agc_rx, 0.01f);

  // Receive raw I and Q samples from HackRF, and put into complex array
  for(int i = 0; i<samplestoaverage; i++) {
    raw_samples[i] = transfer->buffer[2*i] + transfer->buffer[2*i + 1]*I; // Convert to complex
    // raw_samples[i] = (raw_samples[i])*(1.0/128.0); // Scale
    iirfilt_crcf_execute(dcblock, raw_samples[i], &samples[i]); // Remove DC offset
    agc_crcf_execute(agc_rx, samples[i], &agc_output); // Execute AGC
  }
  // printf("Current RSSI: -%f dBm\r\n", agc_crcf_get_rssi(agc_rx));

  pthread_mutex_lock(&rssi_lock);
  rssi = agc_crcf_get_rssi(agc_rx);
  pthread_mutex_unlock(&rssi_lock);
  // Destroy objects
  iirfilt_crcf_destroy(dcblock);
  agc_crcf_destroy(agc_rx);

  return 0;
}


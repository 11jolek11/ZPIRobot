#include "libsdr.h"

static const int samplestoaverage = 1024;

static float rssi;
pthread_mutex_t rssi_lock = PTHREAD_MUTEX_INITIALIZER;

static hackrf_device *device = NULL;
static sdr_module settings;
static int verbose = 0;

void set_verbose_output() {
	verbose = 1;
}

float* get_rssi_p() {
    return &rssi;
}

float get_current_rssi() {
    pthread_mutex_lock(&rssi_lock);
    float temp_rssi = rssi;
    if (verbose) {
    	printf("curr rssi %f dbm real: %f \n", temp_rssi, rssi);
    }
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
  

  pthread_mutex_lock(&rssi_lock);
  rssi = agc_crcf_get_rssi(agc_rx);
  if (CALLBACK_VERBOSE) { 
  	printf("Callback: Current RSSI: -%f dBm\r\n", rssi);
  }
  pthread_mutex_unlock(&rssi_lock);
  // Destroy objects
  iirfilt_crcf_destroy(dcblock);
  agc_crcf_destroy(agc_rx);

  return 0;
}

int init_sdr() {

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

	set_default_settings(&settings);
	settings.device = device;
	settings.rx_callback = rx_callback;

	// set freq gain and bandwidth
	if(verbose) {
		printf("Setting frequency to %llu Hz...", settings.frequency);
	}
	result = hackrf_set_freq(device, settings.frequency);
	if( result != HACKRF_SUCCESS ) {
		printf("hackrf_set_freq() failed: %s (%d)\n", hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}
	result = hackrf_set_lna_gain(device, settings.lna_gain);
	result |= hackrf_set_vga_gain(device, settings.vga_gain);
	if( result != HACKRF_SUCCESS ) {
		printf("hackrf_set_gains failed: %s (%d)\n", hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}
	result = hackrf_set_baseband_filter_bandwidth(device, hackrf_compute_baseband_filter_bw(settings.bandwidth));
	result = hackrf_set_sample_rate(device, settings.bandwidth);
	if( result != HACKRF_SUCCESS ) {
		printf("Setting bandwidth failed: %s (%d)\n", hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}
	return result;
}

int start_rssi_capture() {
         // start receiving, call rx_callback() when data received
          if(verbose) printf("Starting reception...\r\n");
          int start_result = hackrf_start_rx(device, settings.rx_callback, NULL);
          if( start_result != HACKRF_SUCCESS ) {
            printf("hackrf_start_rx() failed: %s (%d)\n", hackrf_error_name(start_result), start_result);
            return EXIT_FAILURE;
          } else {
            if(verbose) printf("hackrf_start_rx() done.\r\n");
          }

	  return start_result;
}

int stop_rssi_capture() {
	int stop_result = hackrf_stop_rx(device);
	if (stop_result != HACKRF_SUCCESS) {
		printf("Failed to stop rx on device! Error code: %d \n%s", stop_result, verbose ? hackrf_error_name(stop_result) : "");
	} else {
		printf("Stop rx \n");
	}
	return stop_result;
}

int cleanup_sdr() {
	if (stop_rssi_capture()) {
		return 1;
	} else if (hackrf_close(device)) {
		printf("Can't stop device! \n");
		return 1;
	}

	pthread_mutex_unlock(&settings.lock);
	pthread_mutex_destroy(&settings.lock);

	hackrf_exit();

	return 0;
}

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

void set_default_settings() {
        settings.frequency = 88600000;
        settings.lna_gain = 32;
        settings.vga_gain = 44;
        settings.bandwidth = 1000000;
        //sdr->lock = PTHREAD_MUTEX_INITIALIZER;

        if (pthread_mutex_init(&settings.lock, NULL) != 0) {
            printf("\n mutex init has failed\n");
        } 
}

int set_new_vga_gain(uint32_t new_vga) {
    pthread_mutex_lock(&settings.lock);
    int device_status_code = hackrf_stop_rx(settings.device);
    int result = hackrf_set_vga_gain(settings.device, new_vga);
    int start_status_code = hackrf_start_rx(settings.device, settings.rx_callback, NULL);

    if (!result) {
        settings.vga_gain = new_vga;
    }

    pthread_mutex_unlock(&settings.lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_lna_gain(uint32_t new_lna) {
    pthread_mutex_lock(&settings.lock);
    int device_status_code = hackrf_stop_rx(settings.device);
    int result = hackrf_set_lna_gain(settings.device, new_lna);
    int start_status_code = hackrf_start_rx(settings.device, settings.rx_callback, NULL);

    if (!result) {
        settings.lna_gain = new_lna;
    }

    pthread_mutex_unlock(&settings.lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_bandwith(double new_bandwidth) {
    pthread_mutex_lock(&settings.lock);
    int device_status_code = hackrf_stop_rx(settings.device);
    int result =  hackrf_set_baseband_filter_bandwidth(settings.device, hackrf_compute_baseband_filter_bw(new_bandwidth));
    int start_status_code = hackrf_start_rx(settings.device, settings.rx_callback, NULL);

    if (!result) {
        settings.bandwidth = new_bandwidth;
    }

    pthread_mutex_unlock(&settings.lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}

int set_new_frequency(uint64_t new_frequency) {
    pthread_mutex_lock(&settings.lock);

    int device_status_code = hackrf_stop_rx(settings.device);
    int result = hackrf_set_freq(settings.device, new_frequency);
    int start_status_code = hackrf_start_rx(settings.device, settings.rx_callback, NULL);

    if (!result) {
        settings.frequency = new_frequency;
    }

    pthread_mutex_unlock(&settings.lock);
    return result | (device_status_code << 1) | (start_status_code << 2);
}
// ######################################################################################################################################
uint32_t get_new_vga_gain() {
    pthread_mutex_lock(&settings.lock);
    uint32_t vga_gain = settings.vga_gain;
    pthread_mutex_unlock(&settings.lock);
    return vga_gain;
}

uint32_t get_new_lna_gain() {
    pthread_mutex_lock(&settings.lock);
    uint32_t lna_gain = settings.lna_gain;
    pthread_mutex_unlock(&settings.lock);
    return lna_gain;
}

double get_new_bandwith() {
    pthread_mutex_lock(&settings.lock);
    double bandwidth = settings.bandwidth;
    pthread_mutex_unlock(&settings.lock);
    return bandwidth;
}

uint64_t get_new_frequency() {
    pthread_mutex_lock(&settings.lock);
    uint64_t freq = settings.frequency;
    pthread_mutex_unlock(&settings.lock);
    return freq;
}


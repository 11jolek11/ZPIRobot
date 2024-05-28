#include "libsdr.h"
#include <unistd.h>
#include <assert.h>

int main() {
	set_verbose_output();
	init_sdr();
	set_default_settings();
	start_rssi_capture();
	for (int i = 0; i <= 5; i++) {
		printf("%g mdb \n", get_current_rssi());
		sleep(1);
	}

	set_new_frequency(88600000);
	assert(get_new_frequency() == 88600000);

	stop_rssi_capture();
	cleanup_sdr();
}


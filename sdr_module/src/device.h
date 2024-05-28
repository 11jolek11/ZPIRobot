#include <libhacrkf/hackrf.h>

struct device_entry {
	int idx;
	hackrf_device_list_t* serial_id;
};

int get_devices_serial_ids_list();




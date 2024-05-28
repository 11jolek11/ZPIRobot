from core import *
import time


set_verbose_output()
init_sdr()
set_default_settings()
start_rssi_capture()

for _ in range(5): 
	print(f"{get_current_rssi()} mdb \n");
	time.sleep(1)

set_new_frequency(88600000)
assert get_new_frequency() == 88600000
stop_rssi_capture()
cleanup_sdr()

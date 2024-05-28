#from core import *
from mock import *
import time

if __name__ == "__main__":
    set_verbose_output()
    init_sdr()
    set_default_settings()
    start_rssi_capture()

    for _ in range(5): 
        print(f"{get_current_rssi()} mdb \n");
        time.sleep(1)

    set_new_frequency(88600001)
    assert get_new_frequency() == 88600001

    set_new_vga_gain(45)
    assert get_new_vga_gain() == 45

    set_new_lna_gain(34)
    assert get_new_lna_gain() == 34


    set_new_bandwith(1000001)
    assert get_new_bandwith() == 1000001

    set_default_settings()
    assert get_new_frequency() == 88600000

    stop_rssi_capture()
    cleanup_sdr()

    print("Test")


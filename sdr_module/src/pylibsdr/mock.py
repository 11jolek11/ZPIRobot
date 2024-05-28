
__verbose = False
__rssi = 0.0

__frequency = 88600000
__vga_gain = 44
__lna_gain = 32
__bandwith = 1000000

def set_verbose_output() -> None:
    global __verbose
    __verbose = True
    return None

def get_current_rssi() -> float:
    global __rssi
    return __rssi

def init_sdr() -> int:
    return 0

def start_rssi_capture() -> int:
    global __rssi
    global __verbose
    __rssi = 134.0

    if __verbose:
        print("Starting reception ...")

    return 0

def stop_rssi_capture() -> int:
    global __rssi
    __rssi = 0.0
    return 0

def cleanup_sdr():
    return 0

def set_default_settings() -> None:
    global __frequency
    global __vga_gain
    global __lna_gain
    global __bandwith

    __frequency = 88600000
    __vga_gain = 44
    __lna_gain = 32
    __bandwith = 1000000.0

    return None

def set_new_vga_gain(vga_gain) -> int :
    global __vga_gain
    __vga_gain = vga_gain
    return 0

def set_new_lna_gain(lna_gain) -> int:
    global __lna_gain
    __lna_gain = lna_gain
    return 0

def set_new_bandwith(bandwith) -> float:
    global __bandwith
    __bandwith = bandwith
    return 0.0

def set_new_frequency(frequency) -> int:
    global __frequency
    __frequency = frequency
    return 0

def get_new_vga_gain() -> int:
    global __vga_gain
    return __vga_gain

def get_new_lna_gain() -> int:
    global __lna_gain
    return __lna_gain

def get_new_bandwith() -> float:
    global __bandwith
    return __bandwith

def get_new_frequency() -> int:
    global __frequency
    return __frequency


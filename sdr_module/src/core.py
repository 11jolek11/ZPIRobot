import ctypes
import os


_file = "libsdr.so"
_path = os.path.join(*(os.path.split(__file__)[:-1] + (_file,)))
_mod = ctypes.cdll.LoadLibrary(_path)

set_verbose_output = _mod.set_verbose_output
set_verbose_output.restype = None

get_current_rssi = _mod.get_current_rssi
get_current_rssi.restype = ctypes.c_float 

init_sdr = _mod.init_sdr
init_sdr.restype = ctypes.c_int

start_rssi_capture = _mod.start_rssi_capture
start_rssi_capture.restype = ctypes.c_int

stop_rssi_capture = _mod.stop_rssi_capture
stop_rssi_capture.restype = ctypes.c_int

cleanup_sdr = _mod.cleanup_sdr
cleanup_sdr.restype = ctypes.c_int 

set_default_settings = _mod.set_default_settings
set_default_settings.restype = None 

set_new_vga_gain = _mod.set_new_vga_gain
set_new_vga_gain.argtypes = (ctypes.c_uint32,)
set_new_vga_gain.restype = ctypes.c_int

set_new_lna_gain = _mod.set_new_lna_gain
set_new_lna_gain.argtypes = (ctypes.c_uint32,)
set_new_lna_gain.restype = ctypes.c_int

set_new_bandwith = _mod.set_new_bandwith
set_new_bandwith.argtypes = (ctypes.c_double,)
set_new_bandwith.restype = ctypes.c_int

set_new_frequency = _mod.set_new_frequency
set_new_frequency.argtypes = (ctypes.c_uint64,)
set_new_frequency.restype = ctypes.c_int

get_new_vga_gain = _mod.get_new_vga_gain
get_new_vga_gain.restype = ctypes.c_uint32 

get_new_lna_gain = _mod.get_new_lna_gain
get_new_lna_gain.restype = ctypes.c_uint32

get_new_bandwith = _mod.get_new_bandwith
get_new_bandwithtype = ctypes.c_double

get_new_frequency = _mod.get_new_frequency
get_new_frequency.restype  = ctypes.c_uint64



















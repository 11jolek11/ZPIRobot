import ctypes

rover = ctypes.CDLL("./rover.so")

move_forward = rover.move
move_forward.argtypes = [ctypes.c_float]
move_forward.restype = ctypes.c_char_p

move_sides = rover.move_sides
move_sides.argtypes = [ctypes.c_bool, ctypes.c_float, ctypes.c_bool, ctypes.c_float]
move_sides.restype = ctypes.c_char_p

send = rover.send
send.argtypes = [ctypes.c_char_p]
send.restype = ctypes.c_char_p

init_serial = rover.init_serial
init_serial.argtypes = [ctypes.c_char_p]
init_serial.restype = ctypes.c_int

close_serial = rover.serial_close_connection


def rover_init_serial(port: str = "/dev/serial0") -> int:
    port = port.encode("utf-8")
    return init_serial(port)


def rover_close_serial() -> None:
    close_serial()


def rover_move_forward(speed: int) -> str:
    # Input speed [0; 100[
    if 0 >= speed or speed >= 100:
        return "Invalid speed value"
    float_speed = speed / 200.0
    return send(move_forward(float_speed))


def rover_move_backward(speed: int) -> str:
    # Input speed [0; 100[
    if 0 >= speed or speed >= 100:
        return "Invalid speed value"
    float_speed = speed / 200.0
    return send(move_forward(-float_speed))


def rover_stop() -> str:
    return send(move_forward(0))


def rover_rotate_left(speed: int) -> str:
    # Input speed [0; 100[
    if 0 >= speed or speed >= 100:
        return "Invalid speed value"
    float_speed = speed / 200.0
    return send(move_sides(True, -float_speed, True, float_speed))


def rover_rotate_right(speed: int) -> str:
    # Input speed [0; 100[
    if 0 >= speed or speed >= 100:
        return "Invalid speed value"
    float_speed = speed / 200.0
    return send(move_sides(True, float_speed, True, -float_speed))




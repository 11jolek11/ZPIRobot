cmake_minimum_required(VERSION 3.27)
project(rover_exe C)

set(CMAKE_C_STANDARD 23)

add_executable(rover_exe main.c
        cJSON.c
        cJSON.h
        libserial.c
        libserial.h
)

CC := gcc

CFLAGS := -Wall -Wextra

INCLUDE_DIR := -L /usr/local/include/ -L. 
LIB_DIR := /lib/arm-linux-gnueabihf/

SHARED_LIBS := -l:libliquid.so -l:libhackrf.so

SRC_DIR := src
TARGET_DIR := target

TARGET := rssi.c server.c

#build:
#	gcc -Wall -Wextra -I /usr/local/include/ -o target/server src/message.c src/callback.c src/rssi.c src/server.c -L /usr/local/lib/ -l:libliquid.so -l:libhackrf.so -l:libcjson.so -lpthread

build:
	gcc -Wall -Wextra -I /usr/local/include/ -fpic -shared -o ./target/libsdr.so ./src/libsdr.c -L /usr/local/lib/ -l:libliquid.so -l:libhackrf.so
	export LD_LIBRARY_PATH=/home/user/hackrf_sec/target

test: ./src/libsdr_test.c
	gcc -Wall -Wextra -I /usr/local/include/ -o ./src/libsdr_test_test ./src/libsdr_test.c -L /usr/local/lib/ -L ./target/ -l:libliquid.so -l:libhackrf.so -l:libsdr.so

UTILS_SRC_DIR := $(SRC_DIR)/utils
UTILS_TARGET_DIR := $(TARGET_DIR)/utils

UTILS_TARGET := rssi_test_probe 

utils: $(UTILS_SRC_DIR)/$(UTILS_TARGET).c
	# $(CC) $(CFLAGS) -o $(TARGET_DIR)/$(TARGET) $(SRC_DIR)/$(TARGET).c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $(UTILS_TARGET_DIR)/$(UTILS_TARGET) $(UTILS_SRC_DIR)/$(UTILS_TARGET).c -L$(LIB_DIR) $(SHARED_LIBS)

# all: build utils
# gcc -Wall -Wextra -I /usr/local/include/ -o target/utils/rssi_test_probe src/utils/rssi_test_probe.c -L /usr/local/lib/ -l:libliquid.so -l:libhackrf.so 




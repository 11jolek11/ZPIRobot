#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios serial;
int fd;
int initialized = 0;

int init_serial(char* tty) {

  printf("Opening %s\n", tty);

  fd = open(tty, O_RDWR | O_EXCL | O_NONBLOCK);

  if (fd == -1) {
    perror(tty);
    return -1;
  }

  if (tcgetattr(fd, &serial) < 0) {
    perror("Getting configuration");
    return -1;
  }

  // Set up Serial Configuration
  serial.c_iflag = 0;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  serial.c_cflag = B115200 | CS8;
  serial.c_cflag |= CLOCAL | CREAD;

  serial.c_cc[VMIN] = 0;
  serial.c_cc[VTIME] = 0;

  serial.c_iflag &= ~(IXON|IXOFF|IXANY);
  serial.c_cflag &= ~(PARENB | PARODD);
  serial.c_cflag &= ~CSTOPB;

  //serial.c_cflag = B115200 | CS8 | CREAD;

  tcsetattr(fd, TCSANOW, &serial); // Apply configuration

  initialized = 1;

  return 1;
}

int serial_send_data(char *to_send) {
  if (initialized != 1) {
    return -1;
  }

  printf("Sending: %s\n", to_send);

  int wcount = write(fd, to_send, strlen(to_send));
  write(fd, "\n", 1);
  if (wcount < 0) {
    perror("Write");
    return -1;
  } else {
    printf("Sent %d characters\n", wcount);
  }
  return 1;
}

char* serial_receive_data(char *buffer) {
  if (initialized != 1) {
    return "Error";
  }
  
  usleep(20000);

  int rcount = read(fd, buffer, sizeof(buffer));
  if (rcount < 0) {
    perror("Read");
    return "";
  } else {
    printf("Received %d characters\n", rcount);
  }

  buffer[rcount] = '\0';

  printf("Received: %s\n", buffer);
  return buffer;
}

char* serial_send_and_receive(char *to_send, char *buffer) {
  
  if (initialized != 1) {
    return "Error";
  }

  printf("Sending: %s\n", to_send);

  int len = strlen(to_send);
  to_send[len] = '\n';
  to_send[len+1] = '\0';
  int wcount = write(fd, to_send, strlen(to_send));
  if (wcount < 0) {
    perror("Write");
    return "";
  } else {
    printf("Sent %d characters\n", wcount);
  }

  usleep(20000);
  
  int rcount = read(fd, buffer, 255);
  if (rcount < 0) {
    perror("Read");
    return "";
  } else {
    printf("Received %d characters\n", rcount);
  }

  buffer[rcount] = '\0';

  printf("Received: %s\n", buffer);
  return buffer;
}

void serial_close_connection() { 
  initialized = 0;
  close(fd); 
}

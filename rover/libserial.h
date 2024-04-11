#ifndef UART_LIBRARY_H
#define UART_LIBRARY_H

int init_serial(char *);

int serial_send_data(char *);

char* serial_receive_data(char *);

char* serial_send_and_receive(char *, char *);

void serial_close_connection();

#endif // UART_LIBRARY_H

#ifndef UART_H
#define UART_H

namespace bglib {

void uart_list_devices();
int uart_find_serialport(const char *name);
int uart_open(const char *port);
void uart_close();
int uart_tx(int len, const unsigned char *data);
int uart_rx(int len, unsigned char *data, int timeout_ms);

} /* namespace bglib */

#endif // UART_H

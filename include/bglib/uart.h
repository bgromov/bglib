#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

void uart_list_devices();
int uart_find_serialport(char *name);
int uart_open(char *port);
void uart_close();
int uart_tx(int len,unsigned char *data);
int uart_rx(int len,unsigned char *data,int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // UART_H

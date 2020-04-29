/*
 * uart.h
 *
 */

#ifndef UART_H_
#define UART_H_

typedef enum {
	UART0 = 0x001C090000,
	UART1 = 0x001C0A0000,
	UART2 = 0x001C0B0000,
	UART3 = 0x001C0C0000} UART_PL011; //Start address of UARTs

//printf dependency:
struct __FILE {
	UART_PL011 	fuart;
};

void enable_uart(UART_PL011 uart);
void write_uart_buf(UART_PL011 uart, const char *buf, int n);
void write_uart_string(UART_PL011 uart, const char *str);
void write_uart_char(UART_PL011 uart, char ch);
void open__stdout();

#endif /* UART_H_ */

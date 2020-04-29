/*
 * uart.c
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "uart.h"

//static int uartdr_offset = 0;
static int uartctl_offset = 0x0030;

void enable_uart(UART_PL011 uart)
{
	short *ctrl = (short*)((int64_t)uart + uartctl_offset);

	*ctrl = 0x0B01;
}

void write_uart_buf(UART_PL011 uart, const char *buf, int n)
{
	int i;
	char *uartdr = (char*)uart;
	for(i=0; i<n; i++)
		*uartdr = buf[i];
}

void write_uart_string(UART_PL011 uart, const char *str)
{
	char ch;
	char *uartdr = (char*)uart;
	while(1){
		ch = *str++;
		if(ch)
			*uartdr = ch;
		else
			break;
	}
}

void write_uart_char(UART_PL011 uart, char ch)
{
	char *uartdr = (char*)uart;
	*uartdr = ch;
}

//printf's dependencies:
FILE  __stdout={UART0};  //Use UART0 as stdout

int fputc(int c, FILE* stream)
{
	uint8_t chr = (uint8_t)c;
	write_uart_char(stream->fuart, chr);
	return chr;
}

int ferror(FILE *stream)
{
	(void)stream;
	return 0;
}

//__stdout must be opened before printf can be called:
void open__stdout()
{
	enable_uart(__stdout.fuart);
}


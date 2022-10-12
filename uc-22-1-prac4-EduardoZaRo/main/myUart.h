#ifndef MY_UART_H
#define MY_UART_H

#include "driver/uart.h"

void uartInit(uint8_t com, uint32_t baudrate, uint8_t size,
uint8_t parity, uint8_t stop);

// Send
void uartPuts(uint8_t com, char *str);
void uartPutchar(uint8_t com, char c);

// Receive
bool uartKbhit(uart_port_t uart_num);
char uartGetchar(uint8_t com);
void uartGets(uint8_t com, char *str);

// Escape sequences
void uartClrScr(uint8_t com);
void uartSetColor(uint8_t com, uint8_t color);
void uartGotoxy(uint8_t com, uint8_t x, uint8_t y);

#define YELLOW  33 // Fixme 
#define GREEN   32 // Fixme 
#define BLUE    34 // Fixme 

// Utils
void myItoa(uint16_t number, char* str, uint8_t base) ;
uint16_t myAtoi(char *str);

#endif
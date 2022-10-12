/* 
https://github.com/uPuC2/uc-22-1-prac4-EduardoZaRo/blob/main/main/Prac4_uart.c
Secuencia de prueba:
Entrada: "12a\n"
Hex->"C"
Bin->"1100"

Entrada: "14.5\n"
Hex->"E"
Bin->"1110"

Entrada: "0\n"
Hex->"0"
Bin->"0"

Entrada: "65535\n"
Hex->"FFFF"
Bin->"1111 ... 1111"

Entrada: "65536\n"
Hex->"0"
Bin->"0"

Entrada: "\n" (Enter)
Hex->"0"
Bin->"0"

Entrada: Presionar '1' por 4 seg, presionar BACKSPACE por 5 seg, e introducir "2\n"
Hex-> "2"
Bin-> "10"

Nota: "\n" equivale a la tecla Enter
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "myUart.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define UARTS_BAUD_RATE         (115200)
#define TASK_STACK_SIZE         (1048)
#define READ_BUF_SIZE           (1024)

void uartInit(uint8_t com, uint32_t baudrate, uint8_t size,
uint8_t parity, uint8_t stop)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = size-5,
        .parity    = parity,
        .stop_bits = stop,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    ESP_ERROR_CHECK(uart_driver_install(com, READ_BUF_SIZE, READ_BUF_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(com, &uart_config));
    if(com == 0)
        ESP_ERROR_CHECK(uart_set_pin(com, 1, 3,
                                    UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    if(com == 1)
        ESP_ERROR_CHECK(uart_set_pin(com, 10, 9,
                                    UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    if(com == 2)
        ESP_ERROR_CHECK(uart_set_pin(com, 17, 16,
                                    UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}



bool uartKbhit(uart_port_t uart_num)
{
    uint8_t length;
    uart_get_buffered_data_len(uart_num, (size_t*)&length);
    return (length > 0);
}



char uartGetchar(uint8_t com)
{
    char c;
    // Wait for a received byte
    while(!uartKbhit(com))
    {
        delayMs(10);
    }
    // read byte, no wait
    uart_read_bytes(com, &c, sizeof(c), 0);
    return c;
}

void uartGets(uint8_t com, char *str){
    int i = 0;
    while(1){
        char a = uartGetchar(com);
        if(a != 13){
            if(a == 127 || a == 8){
                uartPuts(com, "\e[1D");
                uartPuts(com, "\e[0K");
                i--;
                if(i < 0) i = 0;
                str[i] = '\0';
                uart_flush(com);
                uart_flush_input(com);
            }
            else{
                uartPutchar(com,a);
                str[i] = a;
                i++;
            }

        } 
        else break;
    }
    str[i] = '\0';
}
void uartPutchar(uint8_t com, char c)
{
    uart_write_bytes(com, &c, sizeof(c));
}
void uartPuts(uint8_t com, char *str){
    //uart_flush(com);
while(*str != '\0'){
    uartPutchar(com, *str);
    str++;
}
}
void uartClrScr(uint8_t com)
{
    // Uso "const" para sugerir que el contenido del arreglo lo coloque en Flash y no en RAM
    const char caClearScr[6] = "\e[2J";
    uart_write_bytes(com, caClearScr, sizeof(caClearScr));
}
void uartGotoxy(uint8_t com, uint8_t x, uint8_t y)
{
    // Limpie un poco el arreglo de caracteres, los siguientes tres son equivalentes:
     // "\e[1;1H" == "\x1B[1;1H" == {27,'[','1',';','1','H'}
    char x_str[20];
    char y_str[20];
    myItoa(x,x_str, 10);
    myItoa(y,y_str, 10);
    char caGotoxy[50] = "\x1B[";
    strcat(caGotoxy, x_str);
    strcat(caGotoxy, ";");
    strcat(caGotoxy, y_str);
    strcat(caGotoxy, "f");
    uart_write_bytes(com, caGotoxy, sizeof(caGotoxy));
}
void uartSetColor(uint8_t com, uint8_t color){
    //Setcolor
    const char color_str[50];
    myItoa(color,color_str, 10);
    char setColor[50] = "\x1B[";
    strcat(setColor, color_str);
    strcat(setColor, "m");
    uart_write_bytes(com, setColor, sizeof(setColor));
}
void myItoa(uint16_t number, char* str, uint8_t base){
    int base_n[16], i;
    if(number == 0){*str = '0';str++;} 
    for(i = 0; number > 0; i++){
        base_n[i] = number % base;
        number /= base;
    }
    for(i = i - 1; i >= 0 ; i--) { 
        if(base >= 11){
            if(base_n[i] < 10)
                *str = base_n[i] + '0';
            else
                *str = base_n[i] + '7';
        }
        else
            *str = base_n[i]+'0';
        str++;	
    }

    if(*str != '\0')
        *str = '\0';
}
uint16_t myAtoi(char *str){
    //int number[5];
    int number[50], i, j = 0;
    uint16_t n = 0;

    for(i = 0; *str != '\0';i++){
        if(*str-'0' < 10 && *str-'0' >= 0){
            number[i] = *str - '0';
            str++;
        }
        else{
            str++;
            break;
        }
        //if(i == 5) return 0;
            
    }
    for(i = i-1; i >= 0; i--){
        n += (int)pow(10, j)*number[i];
        j++;	
    }

    return n;
}
void app_main(void)
{
    char cad[20];
    char cadUart3[20];
    
    uint16_t num;
    uartInit(0,9600,8,1,2);
    uartInit(1,9600,8,0,1);
    uartInit(2,9600,8,0,1);
    while(1){
        
        uartGetchar(0);
        uartClrScr(0);

        uartGotoxy(0,2,2);
        uartSetColor(0,YELLOW);
        uartPuts(0,"Introduce un numero:");
        
        uartGotoxy(0,22,2);
        uartSetColor(0,GREEN);
        uartGets(0,cad);
// For the following code to work, TX1 must be physically 
// connected with a jumper wire to RX2
// -------------------------------------------
        // Cycle through UART1->UART2
        uartPuts(1,cad);
        uartPuts(1,"\r");
        uartGets(2,cadUart3);
        uartGotoxy(0,3,5);
        uartPuts(0,cadUart3);
// -------------------------------------------
        num = myAtoi(cad);

        myItoa(num,cad,16);
        
        uartGotoxy(0,4,5);
        uartSetColor(0,BLUE);
        uartPuts(0,"Hex: ");
        uartPuts(0,cad);
        myItoa(num,cad,2);
        
        uartGotoxy(0,5,5);
        uartPuts(0,"Bin: ");
        uartPuts(0,cad);
        
    }
}

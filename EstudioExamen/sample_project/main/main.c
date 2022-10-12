#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "string.h"
#include "time.h"
#include "esp_task_wdt.h"
#include "sys/time.h"
#include "esp_timer.h"
#include "soc/gpio_periph.h"
#define _DEF_REG_32b( addr ) ( *(volatile uint32_t *)( addr ) )
#define TIMG0_CONFIG_REG    _DEF_REG_32b( 0x3FF5F000 )
#define TIMG0_TOLO_REG      _DEF_REG_32b( 0x3FF5F004 )
#define TIMG0_TOHI_REG      _DEF_REG_32b( 0x3FF5F008 )
#define TIMG0_TOUPDATE_REG  _DEF_REG_32b( 0x3FF5F00C )
#define TIMG0_TOALARMLO_REG _DEF_REG_32b( 0x3FF5F010 )
#define TIMG0_TOALARMHI_REG _DEF_REG_32b( 0x3FF5F014 )
#define TIMG0_TOLOADLO_REG  _DEF_REG_32b( 0x3FF5F018 )
#define TIMG0_TOLOADHI_REG  _DEF_REG_32b( 0x3FF5F01C )
#define TIMG0_TOLOAD_REG    _DEF_REG_32b( 0x3FF5F020 )
#define TIMG0_INT_ENA_REG    _DEF_REG_32b( 0x3FF5F098 )
#define TIMG0_INT_RAW_REG    _DEF_REG_32b( 0x3FF5F09C )
#define TIMG0_INT_CLR_REG    _DEF_REG_32b( 0x3FF5F0A4 )

#define UART_CONF0_REG      _DEF_REG_32b( 0x3FF40020 )
#define UART_CLKDIV_REG     _DEF_REG_32b( 0x3FF40014 )

#define GPIO_OUT_REG        _DEF_REG_32b( 0x3FF44004 )
#define GPIO_ENABLE_REG     _DEF_REG_32b( 0x3FF44020 )
#define GPIO_OUT_W1TS       _DEF_REG_32b( 0x3FF44008 )
#define GPIO_OUT_W1TC       _DEF_REG_32b( 0x3FF4400C )
#define GPIO_IN_REG         _DEF_REG_32b( 0x3FF4403C )
#define UART_CONF0_REG      _DEF_REG_32b( 0x3FF40020 )
#define UART_CLKDIV1_REG    _DEF_REG_32b( 0x3FF50014 )
#define UART_CLKDIV2_REG    _DEF_REG_32b( 0x3FF6E014 )
extern int duplicarBaudrate(void);
extern void setBaudRate(uint32_t baudrate);
extern int uartInit(uint8_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop);
extern int ror1(int num);
void delayMs(uint16_t ms){
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void app_main(void){

}

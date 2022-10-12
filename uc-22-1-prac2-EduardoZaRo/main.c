/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"


static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO      GPIO_NUM_2
#define delay_raya     300
#define delay_punto     100
static uint8_t s_led_state = 0;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}
void raya(){
        s_led_state = true;
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        vTaskDelay((delay_raya* configTICK_RATE_HZ) / 1000);
}
void punto(){
        s_led_state = true;
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay((delay_punto* configTICK_RATE_HZ) / 1000);
}
void espacio(){
        s_led_state = false;
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay((300* configTICK_RATE_HZ) / 1000);
}
void pausa(){
        s_led_state = false;
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay((50* configTICK_RATE_HZ) / 1000);
}
void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    espacio();
    char cadena[500] = "irvin eduardo zavala roman";
    int i;

    //Se puede poner en un while(1) para que muestre la cadena por tiempo indefinido
    //No puse el while para saber cuando acababa la cadena y obtener el archivo vcd
    for(i = 0; i < strlen(cadena);i++){
        if(cadena[i] == 'a'){punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'b'){raya(); pausa(); punto(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 'c'){raya(); pausa(); punto(); pausa(); raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 'd'){raya(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 'e'){punto(); espacio();}
        if(cadena[i] == 'f'){punto(); pausa(); punto(); pausa(); raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 'g'){raya(); pausa(); raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 'h'){punto(); pausa(); punto(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 'i'){punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 'j'){punto(); pausa(); raya(); pausa(); raya(); pausa(); raya(); espacio();}
        if(cadena[i] == 'k'){raya(); pausa(); punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'l'){punto(); pausa(); raya(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 'm'){raya(); pausa(); raya(); espacio();}
        if(cadena[i] == 'n'){raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 'o'){raya(); pausa(); raya(); pausa(); raya(); espacio();}
        if(cadena[i] == 'p'){punto(); pausa(); raya(); pausa(); raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 'q'){raya(); pausa(); raya(); pausa(); punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'r'){punto(); pausa(); raya(); pausa(); punto(); espacio();}
        if(cadena[i] == 's'){punto(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == 't'){raya(); espacio();}
        if(cadena[i] == 'u'){punto(); pausa(); punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'v'){punto(); pausa(); punto(); pausa(); punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'w'){punto(); pausa(); raya(); pausa(); raya(); espacio();}
        if(cadena[i] == 'x'){raya(); pausa(); punto(); pausa(); punto(); pausa(); raya(); espacio();}
        if(cadena[i] == 'y'){raya(); pausa(); punto(); pausa(); raya(); pausa(); raya(); espacio();}
        if(cadena[i] == 'z'){raya(); pausa(); raya(); pausa(); punto(); pausa(); punto(); espacio();}
        if(cadena[i] == ' '){espacio(); espacio();}
    }
    

}


void setup() {
  app_main();
}

void loop() {
}
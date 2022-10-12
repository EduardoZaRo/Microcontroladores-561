/* Reaction time game */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define LED_GPIO_12    GPIO_NUM_12
#define LED_GPIO_13    GPIO_NUM_13
#define LED_GPIO_14    GPIO_NUM_14
#define LED_GPIO_27    GPIO_NUM_27

#define BTN_GPIO    GPIO_NUM_4
#define BTN_GPIO_5 GPIO_NUM_5
//CAMBIADO A 500ms pq 30 ta bn dificil
#define TIME_WINDOW 500       //300 ms

// Global variable
uint32_t _millis;
uint32_t t1_boton;
uint32_t t2_boton;
uint32_t t1_led;
uint32_t t2_led;
uint32_t t1;
uint32_t t2;
uint32_t start_count=0;
uint16_t randomled; 
uint8_t pulse;
enum eButtonStates
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
};

enum eGameStates
{
    eGameRestart = 0,
    eWaitForStart,
    eStartCount,
    eEndCount,
    eGameOver,
    eYouWin,
    eYouLoose
};

static void initIO(void)
{
    // FIXME:
    // Replace the following code and insert
    // code to initialize all IO pins for the assigment
    gpio_reset_pin(LED_GPIO_12);
    gpio_reset_pin(LED_GPIO_13);
    gpio_reset_pin(LED_GPIO_14);
    gpio_reset_pin(LED_GPIO_27);
    gpio_reset_pin(BTN_GPIO);
    gpio_reset_pin(BTN_GPIO_5);
    
    /* Set LED GPIO as a push/pull output */
    gpio_set_direction(LED_GPIO_12, GPIO_MODE_INPUT);
    gpio_set_direction(LED_GPIO_13, GPIO_MODE_INPUT);
    gpio_set_direction(LED_GPIO_14, GPIO_MODE_INPUT);
    gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
    gpio_set_direction(BTN_GPIO_5, GPIO_MODE_OUTPUT);
    
    /* Set LED GPIO as a push/pull output */
    gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN_GPIO);


}
void resetPins(){
    gpio_reset_pin(LED_GPIO_12);
    gpio_reset_pin(LED_GPIO_13);
    gpio_reset_pin(LED_GPIO_14);
    gpio_reset_pin(LED_GPIO_27);
}
void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
void setZeros(){
    gpio_set_level(LED_GPIO_12, !0); 
    gpio_set_level(LED_GPIO_14, !0);
    gpio_set_level(LED_GPIO_13, !0); 
    gpio_set_level(LED_GPIO_27, !0);
}
void encenderled(uint16_t led){
    switch(led){
        case 1:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_12, !0); 
            gpio_set_level(LED_GPIO_13, !1); 
            gpio_set_level(LED_GPIO_14, !0); 
            gpio_set_level(LED_GPIO_27, !0); 
            break;
        case 2:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_12, !1); 
            gpio_set_level(LED_GPIO_13, !0); 
            break;
        case 3:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_14, !1); 
            gpio_set_level(LED_GPIO_13, !0); 
            break;
        case 4:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_14, !0); 
            gpio_set_level(LED_GPIO_13, !1); 
            break;
        case 5:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_12, !1); 
            gpio_set_level(LED_GPIO_14, !0); 
            break;
        case 6:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_INPUT);
            gpio_set_level(LED_GPIO_12, !0); 
            gpio_set_level(LED_GPIO_14, !1); 
            break;
        case 7:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_OUTPUT);
            gpio_set_level(LED_GPIO_14, !0); 
            gpio_set_level(LED_GPIO_27, !1); 
            break;
        case 8:
            gpio_set_direction(LED_GPIO_12, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_13, GPIO_MODE_INPUT);
            gpio_set_direction(LED_GPIO_14, GPIO_MODE_OUTPUT);
            gpio_set_direction(LED_GPIO_27, GPIO_MODE_OUTPUT);
            gpio_set_level(LED_GPIO_14, !1); 
            gpio_set_level(LED_GPIO_27, !0); 
            break;
        default:
            break;
    }
}



void updateLeds(uint8_t gameState)
{
    // FIXME:
    // Replace the following code and insert
    // code to initialize all IO pins for the assigment
    uint8_t checked = 0;
    if(gameState == eWaitForStart){
        printf("%d\n",_millis);
        if(start_count == 0){
            start_count = 1;
            t1 = _millis;
        }
        int i = 0;
        if(start_count == 1){
            t2 = _millis;
            if(t2-t1 <= 100){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++;setZeros();
            }
            else{
            if(t2-t1 <= 200){
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 300){
            encenderled(1);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 400){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 500){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 600){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 700){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 800){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(8);delayMs(1);_millis++; setZeros();
            }
            else{
            if(t2-t1 <= 900){
            encenderled(1);delayMs(1);_millis++;
            encenderled(2);delayMs(1);_millis++;
            encenderled(3);delayMs(1);_millis++;
            encenderled(4);delayMs(1);_millis++;
            encenderled(5);delayMs(1);_millis++;
            encenderled(6);delayMs(1);_millis++;
            encenderled(7);delayMs(1);_millis++; setZeros();
            }
            else{
            start_count = 0;
            }   
            }      
            }    
            }    
            } 
            }
            }
            }
            }
        }
    }
    if(gameState == eStartCount){
        //eStartCount

        /*
        //TRATAR DE NO USAR DELAY PQ PASAN COSAS
        uint16_t randomled = rand()%8+1;   
        encenderled(randomled);
        delayMs(100);
        */
        if(start_count == 0){
            randomled = rand()%8+1; 
            start_count = 1;
            t1_led = _millis;
        }
        if(start_count == 1){
            t2_led = _millis;
            if(t2_led-t1_led <= 100){
                encenderled(randomled);delayMs(1);_millis++;
            }
            else{
                start_count = 0; encenderled(randomled);
            }
        }
    }
    if(gameState == eYouWin){
        //eYouWin
        if(start_count == 0){
            start_count = 1;
            t1_led = _millis;
        }
        if(start_count == 1){
            t2_led = _millis;
            if(t2_led-t1_led <= 250){
                encenderled(1);delayMs(1);_millis++;
                encenderled(2);delayMs(1);_millis++;
                encenderled(3);delayMs(1);_millis++;
                encenderled(4);delayMs(1);_millis++;
                encenderled(5);delayMs(1);_millis++;
                encenderled(6);delayMs(1);_millis++;
                encenderled(7);delayMs(1);_millis++;
                encenderled(8);delayMs(1);_millis++;
            }
            else{
                setZeros();
                if(t2_led-t1_led <= 500){
                    printf("off\n");
                }
                else{
                    start_count = 0;
                }
            }
        }
    }
    if(gameState == eYouLoose ){
        //eYouLoose
        if(start_count == 0){
            start_count = 1;
            t1_led = _millis;
        }
        if(start_count == 1){
            t2_led = _millis;
            if(t2_led-t1_led <= 500){
                encenderled(1);delayMs(1);_millis++;
                encenderled(2);delayMs(1);_millis++;
                encenderled(3);delayMs(1);_millis++;
                encenderled(4);delayMs(1);_millis++;
            }
            else{
                if(t2_led-t1_led <= 1000){
                    encenderled(5);delayMs(1);_millis++;
                    encenderled(6);delayMs(1);_millis++;
                    encenderled(7);delayMs(1);_millis++;
                    encenderled(8);delayMs(1);_millis++;
                }
                else{
                    start_count = 0;
                }
            }
        }
    }
    if(gameState == eEndCount|| gameState == eGameRestart){
        setZeros();
    }

}
uint8_t checkBtn(void)
{
    // FIXME:
    // Replace the following code and insert
    // code to initialize all IO pins for the assigment
    
    printf("MILLIS %d\tT2-T1 = %d\n", _millis, t2_boton-t1_boton);
    if(!gpio_get_level(BTN_GPIO) && pulse == 0){
        t1_boton = _millis;
        pulse = 1;
    }
    if(pulse == 1 && !gpio_get_level(BTN_GPIO)){
        t2_boton = _millis;
        if(t2_boton-t1_boton >= 1000){
            t2_boton = 0;
            t1_boton = 0;
            return eBtnLongPressed;
        }
        
    }
    if(gpio_get_level(BTN_GPIO)) pulse = 0;
    if(pulse == 0 && t2_boton-t1_boton < 1000 && t2_boton-t1_boton > 30){ t2_boton = 0; t1_boton = 0; return eBtnShortPressed;} 
    return eBtnUndefined;
}
int app_main(void)
{
    uint8_t  currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;
    initIO();
    
    while(1)
    {
        switch(checkBtn())
        {
            case eBtnShortPressed: 
                if(currentGameState== eYouLoose) currentGameState = eYouLoose;
                else{
                if(currentGameState== eYouWin) currentGameState = eYouWin;
                else{currentGameState++; }
                }

                
                break;
            case eBtnLongPressed:  currentGameState = eGameRestart;
                break;
        }
        
        switch(currentGameState)
        {
            case eGameRestart:
                countdown = rand()%2000+500;
                countup = 0;
                currentGameState++;
                break;
            case eWaitForStart:
                break;
            case eStartCount:
                printf("\tCOUNTDOWN %d\n",countdown);
                countdown--;
                if (countdown == 0)
                {
                currentGameState++;
                }
                break;
            case eEndCount:
                if (countdown != 0)
                {
                    currentGameState++;
                }
                else
                {
                    countup++;
                }
                break;
            case eGameOver:
                if ((countdown+countup) > TIME_WINDOW)
                {
                    currentGameState = eYouLoose; 
                }
                else 
                {
                    currentGameState = eYouWin;
                }
                break;
        }
        updateLeds(currentGameState);
        delayMs(1);
        _millis++;
    }
}
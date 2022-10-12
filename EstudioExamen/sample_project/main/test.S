/* .byte 0 coloca espacio entre memoria al ensamblar, por lo que puede solucionar cosas raras al llamar funciones
 aunque byte 0 no se recomienda
Lo correcto es usar .align para alinear codigo*/
#include <xtensa/config/specreg.h>
#include <xtensa/config/core-isa.h>
#include <xtensa/corebits.h>
    .data

    .global vone
    vone:
        .int 0xAA

    .set voneaddr, vone
        .text
        .align
        .global setBaudRate,duplicarBaudrate,uartInit, ror1
ror1:
    entry sp,32
    /*
        PARAMETRO
            a2: numero a rotar 1 bit a la izquierda
    */
    movi a15, 0x80000000
    and a14, a2, a15
    slli a2, a2, 1
    movi a14, 31
    SSR a14
    srl a15, a15
    or a2, a2, a15
    retw
    .align 4

setBaudRate:
    /*
        PARAMETROS 
            a2: uint32_t baudrate
    */
    entry sp, 32
    mov a13, a2        /*Baud rate*/
    movi a12,1280000000/*80Mhz x 16*/
    QUOS a11, a12, a13 /*a12/115200*/

    movi a10, 0xF
    and a10, a11, a10  /*Parte decimal*/
    srli a11, a11, 4   /*Parte entera*/
    movi a9, 0x3FF40014/*Direccion clkdiv*/
    mov a8, a11
    slli a10, a10, 20
    or a8, a8, a10     /*Valor completo del registro clkdiv*/
    s32i a8, a9, 0     /*Se guarda el valor en la direccion del clkdiv*/
    retw
    .align 4




    
duplicarBaudrate:
    /*
        NO RECIBE PARAMETROS
    */
    entry sp, 32

    movi a15, 0x3FF40014    
    l32i a14, a15, 0        
    movi a13, 0xFFFFF     
    and a14, a14, a13       
    slli a14, a14, 4   
    mov a13, a14    

    l32i a14, a15, 0       
    srli a14, a14, 15 
    srli a14, a14, 5
    movi a12, 0xF
    and a14, a14, a12
    mov a12, a14

    movi a11, 1280000000
    or a13, a13, a12
    QUOS a11, a11, a13 
    mov a10, a11 /*baudrate*/

    slli a15, a10, 1     /*baudrate duplicado*/
    movi a14, 1280000000 
    QUOS a15, a14, a15   /*clkdiv*/
    movi a14, 0xF        
    and a14, a15, a14    /*clkdiv_frac*/
    slli a14, a14, 20    /*clkdiv_frac << 20*/
    srli a15, a15, 4     /*clkdiv >> 4 = clkdiv_int*/
    or a15, a15, a14
    movi a9, 0x3FF40014    
    s32i a15, a9, 0
    retw
    .align 4

uartInit:

    /*
        ---------PARAMETROS---------
                a2: uart_num
                a3: baudrate
                a4: size
                a5: parity
                a6: stop
    */
    entry sp, 32
    movi a15, 0         /*Mascara para clkdiv*/
    movi a14, 1280000000
    QUOS a14, a14, a3   /* 80MHz*16 / baudrate */ 
    movi a10, 0xF
    and a10, a14, a10   /*Parte decimal*/
    srli a14, a14, 4    /*Parte entera*/
    mov a8, a14
    slli a10, a10, 20
    or a8, a8, a10      /*Valor completo del registro clkdiv*/


    xor a15, a15, a15   /*Mascara para uart_conf*/
    xor a14, a14, a14 
    beqi a5, 0, .finSetParidad
    beqi a5, 1, .setParidadImpar
    beqi a5, 2, .setParidadPar
    j .fin
.setParidadImpar:
    movi a14, 3
    or a15, a15, a14
    j .finSetParidad
.setParidadPar:
    movi a14, 2
    or a15, a15, a14
    j .finSetParidad  
.finSetParidad:
    /*PARIDAD CONFIGURADA*/
    addi a4, a4, -5
    slli a4, a4, 2 /* UART_BIT_NUM */
    or a15, a15, a4
    /*BIT SIZE CONFIGURADO*/
    slli a6, a6, 4
    or a15, a15, a6
    /*STOP BIT CONFIGURADO*/

    movi a11, 0x3FF40020    /*DIR UART_CONF0_REG*/
    movi a12, 0xFFFFFFC0    /*Mascara para borrar bits a configurar*/
    l32i a10, a11, 0
    and a10, a10, a12
    or a10, a10, a15
    s32i a10, a11, 0

    
    beqi a2, 0, .setCLKDIV_UART0
    beqi a2, 1, .setCLKDIV_UART1
    beqi a2, 2, .setCLKDIV_UART2
    j .fin
.setCLKDIV_UART0:
    movi a9, 0x3FF40014
    s32i a8, a9, 0   
    j .finSetCLKDIV  
.setCLKDIV_UART1:
    movi a9, 0x3FF50014
    s32i a8, a9, 0
    j .finSetCLKDIV  
.setCLKDIV_UART2:
    movi a9, 0x3FF60014
    s32i a8, a9, 0
    j .finSetCLKDIV    
.finSetCLKDIV:

.fin:
    movi a2, 10
    neg a2, a2
    addi a2, a2, -1
    retw
    .align 4


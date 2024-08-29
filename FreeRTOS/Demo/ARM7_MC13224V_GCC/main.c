/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * This is a simple main that will start the FreeRTOS-Kernel and run a periodic
 * task that only delays if compiled with the template port, this project will
 * do nothing. For more information on getting started please look here:
 * https://freertos.org/FreeRTOS-quick-start-guide.html
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
// #include <queue.h>
// #include <semphr.h>
// #include <timers.h>
#include <task.h>
#include "flash.h"
#include "partest.h"

/* Standard includes. */
#include <stdio.h>

/*-----------------------------------------------------------*/
// #define TASK_RED
// #define TASK_GREEN
// #define TASK_MATH
#define TASK_LEDS
#define FIBONACCI

/*-----------------------------------------------------------*/

#define DEFAULT_PRIORITY (configMAX_PRIORITIES - 1U)
#define xDelay (TickType_t) (500/portTICK_PERIOD_MS)

#define LED_RED     gpio_pin_44
#define LED_GREEN   gpio_pin_45

/*-----------------------------------------------------------*/
// Task 1:
#ifdef TASK_RED
static void vBlinkRed(void *parameters);
#endif

// Task 2:
#ifdef TASK_GREEN
static void vBlinkGreen(void *parameters);
#endif

// Task 3:
#ifdef FIBONACCI
static void vFibonacciTask(void *parameters);
#endif

/*-----------------------------------------------------------*/

static inline void print_str(char * str){
    uart_send(UART1_ID, str, strlen(str));
}

void vCrash(){
    portENTER_CRITICAL();
    gpio_clear_pin(LED_GREEN);
    print_str("\r\nCRASH CRASH CRASH\r\n");
    for(;;){
        gpio_set_pin(LED_GREEN);
        gpio_set_pin(LED_RED);

        for(uint32_t i=0; i<10000; i++);

        gpio_clear_pin(LED_GREEN);
        gpio_clear_pin(LED_RED);
    }
    portEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/
void launchTasks(){

    BaseType_t aux;

#ifdef TASK_RED
    aux = xTaskCreate( vBlinkRed,
                      "BlinkyRed",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      DEFAULT_PRIORITY,
                      ( TaskHandle_t * ) NULL);

    if (aux != pdPASS)
    {
        print_str("Fallo al crear BlinkyRed\r\n");
        vCrash();
    }
    else {
        print_str("Task BlinkyRed Start\r\n");
    }
#endif //TASK_RED

#ifdef TASK_GREEN
    aux = xTaskCreate( vBlinkGreen,
                      "BlinkyGreen",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      DEFAULT_PRIORITY,
                      ( TaskHandle_t * ) NULL);

    if (aux != pdPASS)
    {
        print_str("Fallo al crear BlinkyGreen\r\n");
        vCrash();
    }
    else {
        print_str("Task BlinkyGreen Start\r\n");
    }
#endif //TASK_GREEN

#ifdef TASK_LEDS
    vStartLEDFlashTasks( DEFAULT_PRIORITY - 1U );
#endif

#ifdef TASK_MATH
    vStartIntegerMathTasks( DEFAULT_PRIORITY );
#endif

#ifdef FIBONACCI
    aux = xTaskCreate( vFibonacciTask,
                      "Fibonacci",
                      configMINIMAL_STACK_SIZE*2,
                      NULL,
                      DEFAULT_PRIORITY,
                      NULL);
    if (aux != pdPASS)
    {
        print_str("Fallo al crear Fibonacci\r\n");
        vCrash();
    }
    else {
        print_str("Task Fibonacci Start\r\n");
    }
#endif //FIBONACCI


}
/*-----------------------------------------------------------*/

void main(void) {
    portENTER_CRITICAL();
    // iprintf("Example FreeRTOS Project\r\n");
    print_str("\r\n\n\nExample FreeRTOS Project\r\n");

    excep_set_handler(excep_undef, vCrash);

    vParTestInitialise();

    vParTestSetLED( LED_GREEN, 0 );
    portEXIT_CRITICAL();

    // for(uint32_t i=0; i<100000; i++);

    launchTasks();

    // for(uint32_t i=0; i<100000; i++);


    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Should not reach here. */
    gpio_clear_pin(LED_GREEN);
    vCrash();
}

/*-----------------------------------------------------------*/

#if (configCHECK_FOR_STACK_OVERFLOW > 0)

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    /* Check pcTaskName for the name of the offending task,
   * or pxCurrentTCB if pcTaskName has itself been corrupted. */
    // (void)xTask;

    portENTER_CRITICAL();
    print_str("\r\nSTACK OVERFLOW\r\n");
    print_str(pcTaskName);
    print_str("\r\nSTACK OVERFLOW\r\n");
    gpio_clear_pin(LED_GREEN);
    gpio_clear_pin(LED_RED);
    vCrash();
    portEXIT_CRITICAL();
}

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
/*-----------------------------------------------------------*/

#ifdef FIBONACCI
#define STR_LEN 8
static inline void print_int(uint32_t num)
{
    char str[STR_LEN];
    uint8_t i=0;

    // Convertimos los dígitos a caracteres
    while (num != 0 && i < STR_LEN-3) {
        uint32_t aux = num % 10;
        str[i++] = aux + '0';
        num = num / 10;
    }

    // // No deberíamos de habernos pasado,
    // // pero así me quedo más tranquilo
    // if( i+2 >= STR_LEN ) vCrash();

    // Terminamos la cadena con 3 caracteres:
    // Retorno de carro, nueva linea, caracter nulo.
    str[i] = '\r';
    str[i+1] = '\n';
    str[i+2] = '\0';

    // Invertimos la cadena
    for (uint8_t j = 0; j < i / 2; j++) {
        char aux = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = aux;
    }


    uart_send(UART1_ID, str, strlen(str));
}


static void vFibonacciTask(void *parameters) 
{
    static uint16_t a=0, b=1, aux=1;
    TickType_t xLastTime;

    xLastTime = xTaskGetTickCount();

    for(;;){
        portENTER_CRITICAL();
        print_int(b);
        portEXIT_CRITICAL();

        vTaskDelayUntil(&xLastTime, aux);

        portENTER_CRITICAL();
        aux=a+b;
        a=b;
        b=aux;
        if (b > 10000){
            a=0; aux=b=1;
        }
        aux = aux/portTICK_PERIOD_MS;
        portEXIT_CRITICAL();

        vTaskDelayUntil(&xLastTime, aux);
    }
}
#endif // FIBONACCI

/*-----------------------------------------------------------*/
#ifdef TASK_RED
static void vBlinkRed(void *parameters) {

    TickType_t xLastTime;
    xLastTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastTime, xDelay);
        portENTER_CRITICAL();
        vParTestToggleLED(LED_RED);
        portEXIT_CRITICAL();
        // print_str("ROJO\r\n");
    }
    vTaskDelete(NULL);
    vCrash();

    /* Unused parameters. */
    (void)parameters;
}
#endif //TASK_RED

#ifdef TASK_GREEN
static void vBlinkGreen(void *parameters) {
    TickType_t xLastTime;
    xLastTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastTime, xDelay*2);
        portENTER_CRITICAL();
        vParTestToggleLED(LED_GREEN);
        portEXIT_CRITICAL();
        // print_str("VERDE\r\n");
    }
    vTaskDelete(NULL);
    vCrash();

    /* Unused parameters. */
    (void)parameters;
}
#endif //TASK_GREEN

/*-----------------------------------------------------------*/

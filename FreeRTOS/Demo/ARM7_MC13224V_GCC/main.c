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
#include "partest.h"

/* Standard includes. */
#include <stdio.h>

/*-----------------------------------------------------------*/

// #define TASK_RED
// #define TASK_GREEN
// #define TASK_MATH
#define TASK_LEDS

/*-----------------------------------------------------------*/

#define xDelay (TickType_t) 500/portTICK_PERIOD_MS

#define LED_RED gpio_pin_44
#define LED_GREEN gpio_pin_45

void vStartLEDFlashTasks( UBaseType_t uxPriority );

void toggle_led(gpio_pin_t LED) {
    static uint8_t red_led_state   = 0;
    static uint8_t green_led_state = 0;

    uint8_t *led_state = LED == LED_RED ? &red_led_state : &green_led_state;

    if (*led_state)
        gpio_clear_pin(LED);
    else
        gpio_set_pin(LED);

    *led_state = ~(*led_state);
}

static inline void print_str(char * str)
{
    uart_send(UART1_ID, str, strlen(str));
}

/*-----------------------------------------------------------*/
// Task 1:
static void vBlinkRed(void *parameters) {
    portENTER_CRITICAL();

    for (;;) {
        portENTER_CRITICAL();
        toggle_led(LED_RED);
        print_str("ROJO\r\n");
        vTaskDelay(xDelay); /* delay 100 ticks */
        portEXIT_CRITICAL() 
    }
    vTaskDelete(NULL);

    /* Unused parameters. */
    (void)parameters;
}

// Task 2:
static void vBlinkGreen(void *parameters) {
    portENTER_CRITICAL();

    for (;;) {
        portENTER_CRITICAL();
        toggle_led(LED_GREEN);
        print_str("VERDE\r\n");
        vTaskDelay(xDelay*2); /* delay X ticks */
        portEXIT_CRITICAL() ;
    }
    vTaskDelete(NULL);

    /* Unused parameters. */
    (void)parameters;
}
/*-----------------------------------------------------------*/
void launchTasks(){

    BaseType_t aux;

    #ifdef TASK_RED
    aux = xTaskCreate( vBlinkRed,
                      "BlinkyRed",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      configMAX_PRIORITIES - 1U,
                      NULL);
    if (aux != pdPASS)
    {
        print_str("Fallo al crear BlinkyRed\r\n");
        gpio_clear_pin(LED_GREEN);
        for(;;){}
    }
    else {
        // iprintf("Task BlinkyRed creada con éxito\r\n");
        print_str("Task BlinkyRed creada con éxito\r\n");
    }
    #endif

    #ifdef TASK_GREEN
    aux = xTaskCreate( vBlinkGreen,
                      "BlinkyGreen",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      configMAX_PRIORITIES - 2U,
                      NULL);
    if (aux != pdPASS)
    {
        print_str("Fallo al crear BlinkyGreen\r\n");
        gpio_clear_pin(LED_GREEN);
        for(;;){}
    }
    else {
        print_str("Task BlinkyGreen creada con éxito\r\n");
    }
    #endif

    #ifdef TASK_LEDS
    vStartLEDFlashTasks( configMAX_PRIORITIES - 2U );
    #endif

    #ifdef TASK_MATH
    vStartIntegerMathTasks( configMAX_PRIORITIES - 1U );
    #endif


}
/*-----------------------------------------------------------*/

void main(void) {
    // iprintf("Example FreeRTOS Project\r\n");
    print_str("\r\n\r\nExample FreeRTOS Project\r\n");

    // led_init();
    vParTestInitialise();

    vParTestSetLED( LED_GREEN, 0 );

    launchTasks();

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Should not reach here. */
    gpio_clear_pin(LED_GREEN);
    for (;;) {}
}
/*-----------------------------------------------------------*/

#if (configCHECK_FOR_STACK_OVERFLOW > 0)

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    /* Check pcTaskName for the name of the offending task,
   * or pxCurrentTCB if pcTaskName has itself been corrupted. */
    (void)xTask;
    (void)pcTaskName;
}

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
/*-----------------------------------------------------------*/

/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
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
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>
#include "partest.h"

/* Standard includes. */
#include <stdio.h>

/*-----------------------------------------------------------*/

#define LED_RED gpio_pin_44
#define LED_GREEN gpio_pin_45
#define TICKS_DELAY 20
// const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

/*-----------------------------------------------------------*/

void led_init() {
    gpio_set_pin_dir_output(LED_RED);
    gpio_clear_pin(LED_RED);

    gpio_set_pin_dir_output(LED_GREEN);
    gpio_clear_pin(LED_GREEN);
}

void toggle_led(gpio_pin_t LED) {
    static uint32_t red_led_state   = 0;
    static uint32_t green_led_state = 0;

    uint32_t *led_state = LED == LED_RED ? &red_led_state : &green_led_state;

    if (*led_state)
        gpio_clear_pin(LED);
    else
        gpio_set_pin(LED);

    *led_state = !*(led_state);
}


/*-----------------------------------------------------------*/

// Task 1:
static void blinkRed(void *parameters) {
    /* Unused parameters. */
    (void)parameters;

    /* Block for 1 sec. */
    for (;;) {
        toggle_led(LED_RED);
        vTaskDelay(TICKS_DELAY); /* delay 100 ticks */
    }
}

// Task 2:
static void blinkGreen(void *parameters) {
    /* Unused parameters. */
    (void)parameters;

    /* Block for 1 sec. */
    // const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
    for (;;) {
        toggle_led(LED_RED);
        vTaskDelay(TICKS_DELAY); /* delay X ticks */
    }
}
/*-----------------------------------------------------------*/

void main(void) {
    (void)printf("Example FreeRTOS Project\n");

    led_init();

    gpio_set_pin(LED_GREEN);

    BaseType_t aux = xTaskCreate( blinkRed,
                                 "BlinkRed",
                                 configMINIMAL_STACK_SIZE*4,
                                 NULL,
                                 configMAX_PRIORITIES - 1U,
                                 NULL);

    if (aux != pdPASS)
    {
        gpio_clear_pin(LED_GREEN);
        for(;;){}
    }

    // (void)xTaskCreate( blinkGreen,
    //                   "BlinkGreen",
    //                   configMINIMAL_STACK_SIZE*2,
    //                   NULL,
    //                   configMAX_PRIORITIES - 1U,
    //                   NULL);


    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Should not reach here. */
    gpio_clear_pin(LED_GREEN);
    for (;;) {
    }
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

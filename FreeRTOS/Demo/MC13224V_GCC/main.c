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

// /* Dirección del registro de control de dirección del GPIO32-GPIO63 */
// volatile uint32_t *const reg_gpio_pad_dir1 = (uint32_t *)0x80000004;
//
// /* Dirección del registro de activación de bits del GPIO32-GPIO63 */
// volatile uint32_t *const reg_gpio_data_set1 = (uint32_t *)0x8000004c;
//
// /* Dirección del registro de limpieza de bits del GPIO32-GPIO63 */
// volatile uint32_t *const reg_gpio_data_reset1 = (uint32_t *)0x80000054;
//
/* El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1) */
uint32_t const led_red_mask = (1 << (44 - 32));

/*-----------------------------------------------------------*/

static void exampleTask(void *parameters);

/*-----------------------------------------------------------*/

static void exampleTask(void *parameters) {
  /* Unused parameters. */
  (void)parameters;

  for (;;) {
    vParTestToggleLED(led_red_mask);
    vTaskDelay(500); /* delay 100 ticks */
  }
}
/*-----------------------------------------------------------*/

void main(void) {
  static StaticTask_t exampleTaskTCB;
  static StackType_t exampleTaskStack[configMINIMAL_STACK_SIZE];

  (void)printf("Example FreeRTOS Project\n");

  (void)xTaskCreateStatic(exampleTask, "example", configMINIMAL_STACK_SIZE,
                          NULL, configMAX_PRIORITIES - 1U,
                          &(exampleTaskStack[0]), &(exampleTaskTCB));

  /* Start the scheduler. */
  vTaskStartScheduler();

  for (;;) {
    /* Should not reach here. */
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

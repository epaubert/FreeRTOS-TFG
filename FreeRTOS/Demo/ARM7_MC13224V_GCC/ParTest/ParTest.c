/*
 * FreeRTOS V202212.00 Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
	Changes from V2.5.2
		
	+ All LED's are turned off to start.
*/


#include "FreeRTOS.h"
#include "partest.h"

#define LED_RED     gpio_pin_44
#define LED_GREEN   gpio_pin_45

/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *-----------------------------------------------------------*/

static inline void print_str(char * str){
    uart_send(UART1_ID, str, strlen(str));
}

void vParTestInitialise( void )
{
    // print_str("vParTestInitialise\r\n");
    gpio_set_pin_dir_output(LED_RED);
    gpio_clear_pin(LED_RED);

    gpio_set_pin_dir_output(LED_GREEN);
    gpio_clear_pin(LED_GREEN);
}
/*-----------------------------------------------------------*/

inline void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
    // if (uxLED >= LED_RED && uxLED <= LED_GREEN ){
        portENTER_CRITICAL();
        if (xValue)
            gpio_clear_pin(uxLED);
        else
            gpio_set_pin(uxLED);
        portEXIT_CRITICAL();
    // }
    // else {
    //     print_str("ERROR vParTestSetLED\r\n");
    // }
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
    static uint8_t red_led_state   = 0;
    static uint8_t green_led_state = 0;

    switch (uxLED) {
        case LED_RED:
            // print_str("Rojo Toggle\r\n");
            // break;
        case LED_GREEN:
            // print_str("Verde Toggle\r\n");
            break;
        default:
            print_str("ERROR Toggle\r\n");
            return;
    }

    uint8_t *led_state = uxLED == LED_RED ? &red_led_state : &green_led_state;

    vParTestSetLED(uxLED, *led_state);

    *led_state = !*(led_state);
}


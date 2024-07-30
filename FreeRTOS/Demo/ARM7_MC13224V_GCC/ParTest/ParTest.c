/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#define LED_RED gpio_pin_44
#define LED_GREEN gpio_pin_45

#define partstFIRST_IO			gpio_pin_44
#define partstNUM_LEDS			( 2 )

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


/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *-----------------------------------------------------------*/

void vParTestInitialise( void )
{
    led_init();
}
/*-----------------------------------------------------------*/

void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
    gpio_set_pin(LED_RED);
    gpio_set_pin(LED_GREEN);
}
/*-----------------------------------------------------------*/

inline void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
    toggle_led(uxLED);
}


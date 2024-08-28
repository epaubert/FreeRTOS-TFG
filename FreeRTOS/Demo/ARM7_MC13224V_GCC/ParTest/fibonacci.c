
#include <FreeRTOS.h>
#define STR_LEN 8

static inline void print_int(uint32_t num)
{
    char str[STR_LEN];
    uint32_t i=0;

    // Convertimos los dígitos a caracteres
    while (num != 0 && i < STR_LEN-1) {
        uint32_t aux = num % 10;
        str[i++] = aux + '0';
        num = num / 10;
    }

    // Terminamos la cadena con 3 caracteres
    str[i] = '\r';
    str[i+1] = '\n';
    str[i+2] = '\0';

    // Invertimos la cadena
    for (uint32_t j = 0; j < i / 2; j++) {
        char aux = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = aux;
    }

    uart_send(UART1_ID, str, strlen(str));
}


static void vFibonacciTask(void *parameters) 
{
    static uint32_t a=0, b=1;
    TickType_t xLastTime;

    xLastTime = xTaskGetTickCount();

    for(;;){
        portENTER_CRITICAL();
        print_int(b);
        portEXIT_CRITICAL();

        vTaskDelayUntil(&xLastTime, b);

        portENTER_CRITICAL();
        uint32_t aux=a+b;
        a=b;
        b=aux;
        if (b > 10000){
            a=0; b=1;
        }
        portEXIT_CRITICAL();

        vTaskDelayUntil(&xLastTime, b);
    }
}

# Problemas
- [X] Compilar
    - [X] CMake para el bsp
    - Modificado el archivo bsp/hal/excep.c para que todas las funciones se compilen siempre para ARM, no thumb => __attribute__((target("arm")))
    - [X] CMake para el resto del projecto
- [X] Linker
    - [X] no econtraba el bsp

* Tamaño -> thumb y -Os


- [X] Una vez configurado el port al ejecutar y debuggear con gdb llegué a que FreeRTOS no tenía suficiente RAM para crear la task así que la aumenté de 4096 a 16384. SOLUCIONADO
- [X] Ahora tengo un problema que puede estar relacionado con el stack disponible para la task. SOLUCIONADO
- [X] Parece que tengo que implementar un handler para el SWI (Software interrupt). SOLUCIONADO
- [ ] Es posible que no se estén reactivando las interrupciones, lo que hace que se quede en la idle task permanentemente
    <!-- * cpsr = 0x20000092 -->

Puede que se esté interrumpiendo blinkRed con un tick y no tenga tiempo para hacer suficiente entre ticks o no se esté restaurando el contexto correctamente



hay un problema de corrupción de memoria, comparar los registros en las llamadas a increment tick

# Problemas
- [X] Compilar
    - [X] CMake para el bsp
    - Modificado el archivo bsp/hal/excep.c para que todas las funciones se compilen siempre para ARM, no thumb => __attribute__((target("arm")))
    - [X] CMake para el resto del projecto
- [X] Linker
    - [X] no econtraba el bsp

* Tamaño -> thumb y -Os

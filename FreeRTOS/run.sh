#!/bin/bash

SRAM_BASE=0x00400000
SERIAL_PORT=/dev/ttyUSB1
BAUDRATE=115200
TELNET_PORT=4444
TCL_PORT=6666
GDB_PORT=3333
TOOLS_PATH=.


# TERMINAL=putty -serial -sercfg $BAUDRATE $SERIAL_PORT

MC1322X_LOAD=$TOOLS_PATH/bin/mc1322x-load
FLASHER=$TOOLS_PATH/flasher_redbee-econotag.bin
BBMC=$TOOLS_PATH/bin/bbmc

ELF=bin/Demo.elf
BIN=bin/$(basename $ELF .elf).bin

check_bin(){
    echo "Buscando $ELF..."
    cmake --fresh
    make clean
    echo "Generando $ELF..."
    make

    if ! test -f $ELF ; then
        echo "Fallo al compilar $ELF"
        exit 1
    fi

    echo "Generando $BIN..."
    arm-none-eabi-objcopy -O binary $ELF $BIN
}

check_openocd(){
    if [ ! `pgrep openocd` ]; then
        echo "Lanzando openocd ..."
        xterm -e "openocd -f interface/ftdi/redbee-econotag.cfg -f board/redbee.cfg" &
        sleep 1
    fi
}

run(){
    echo "Ejecutando el programa..."
    echo -e "soft_reset_halt\n load_image $BIN $SRAM_BASE\n resume $SRAM_BASE" | nc -i 1 localhost $TELNET_PORT > /dev/null
}

debug(){
    sleep 2 && echo "Lanzando Debugger..."
    gdb -x break.gdb | tee gdb.log
}


check_bin
check_openocd
run &
debug

# $MC1322X_LOAD -f $BIN -t $SERIAL_PORT
# $TERMINAL -e "telnet localhost $TELNET_PORT" &

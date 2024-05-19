#!/bin/bash

SRAM_BASE=0x00400000
SERIAL_PORT=/dev/ttyUSB1
BAUDRATE=115200
TOOLS_PATH=.

# TERMINAL=putty -serial -sercfg $BAUDRATE $SERIAL_PORT

MC1322X_LOAD=$TOOLS_PATH/bin/mc1322x-load
FLASHER=$TOOLS_PATH/flasher_redbee-econotag.bin
BBMC=$TOOLS_PATH/bin/bbmc

ELF=bin/Demo
BIN=$ELF.bin

check_bin(){
    echo "Buscando $ELF..."
    if ! test -f $ELF ; then
        echo "$ELF no encontrado"
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
    echo -e "soft_reset_halt\n load_image $BIN $SRAM_BASE\n resume $SRAM_BASE" | nc -i 1 localhost 3333  > /dev/null
}


check_bin
check_openocd
run &

sleep 2 && gdb -x gdb.txt

# $MC1322X_LOAD -f $BIN -t $SERIAL_PORT
# $TERMINAL -e "telnet localhost 3333" &

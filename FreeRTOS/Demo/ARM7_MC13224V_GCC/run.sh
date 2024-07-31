#!/run/current-system/sw/bin/bash

SRAM_BASE=0x00400000
SERIAL_PORT=/dev/ttyUSB1
BAUDRATE=115200
TELNET_PORT=4444
TCL_PORT=6666
GDB_PORT=3333
TOOLS_PATH=tools_econotag
FREERTOS_PATH=../..

MC1322X_LOAD=$TOOLS_PATH/bin/mc1322x-load
FLASHER=$TOOLS_PATH/flasher_redbee-econotag.bin
BBMC=$TOOLS_PATH/bin/bbmc

OPENOCD="openocd -f interface/ftdi/redbee-econotag.cfg -f board/redbee.cfg"

ELF=$FREERTOS_PATH/bin/Demo.elf
BIN=$FREERTOS_PATH/bin/$(basename $ELF .elf).bin
OBJDUM=Demo.dump


check_bin(){
    # echo "Limpiando $ELF..."
    # rm $ELF
    echo "Generando $ELF..."
    cd $FREERTOS_PATH
    # make clean && sleep 1
    make 
    cd -

    sleep 2

    if ! test -f $ELF ; then
        echo "Fallo al compilar $ELF"
        exit 1
    fi

    echo "Generando $BIN..."
    arm-none-eabi-objcopy -O binary $ELF $BIN

    echo "Generando $OBJDUMP..."
    arm-none-eabi-objdump -D $ELF > $OBJDUM
}

check_openocd(){
    if [ ! `pgrep openocd` ]; then
        echo "Lanzando openocd ..."
        xterm -e $OPENOCD &
        sleep 1
    fi
}

check_putty(){
    if [ ! `pgrep putty` ]; then
        echo "Lanzando putty ..."
        putty -serial -sercfg $BAUDRATE $SERIAL_PORT -fn "Monospace 12"&
        sleep 1
    fi
}

run(){
    echo "Ejecutando el programa..."
    echo -e "soft_reset_halt\n load_image $BIN $SRAM_BASE\n resume $SRAM_BASE" | nc -i 1 localhost $TELNET_PORT > /dev/null
}

debug(){
    sleep 2 && echo "Lanzando Debugger..."
    gdb -q -x break.gdb
}

check_bin
check_openocd
check_putty
run &
debug

# $MC1322X_LOAD -f $BIN -t $SERIAL_PORT
# $TERMINAL -e "telnet localhost $TELNET_PORT" &

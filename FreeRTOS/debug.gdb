set architecture arm
target extended-remote localhost:3333
monitor soft_reset_halt
file bin/Demo.elf
load bin/Demo.elf
# break _start
break main
# break vTaskStartScheduler
# break xPortStartScheduler
# break vPortISRStartFirstTask
# break vTickISR
# break blinkRed
# break vTaskDelay
# break blinkRed
# break vTaskDelay

# break xTaskIncrementTick

# break vPortYieldProcessor
# break xTaskResumeAll

# 1
break vTaskSwitchContext
commands
    silent
    printf "vTaskSwitchContext called\n"
    info registers cpsr
    continue
end

# 1
break vPortYieldProcessor
commands
    silent
    printf "vPortYieldProcessor called\n"
    info registers cpsr
    continue
end

break vTickISR
commands
    silent
    printf "vTickISR called\n"
    info registers cpsr
    continue
end

break xTaskIncrementTick
commands
    silent
    printf "xTaskIncrementTick called\n"
    print xTickCount
    info registers cpsr
    continue
end

# 1
break blinkRed
commands
    silent
    printf "blinkRed called\n"
    info registers cpsr
    continue
end

# break blinkGreen
# commands
#     printf "blinkGreen called\n"
#     # info registers cpsr
#     continue
# end

# TODO: hacer un watch en los bits de interrupciones
# watch *(volatile uint32_t*)0xADDRESS
# commands
#     silent
#     printf "Interrupt bit changed\n"
#     continue
# end

# Layout
# layout split

continue

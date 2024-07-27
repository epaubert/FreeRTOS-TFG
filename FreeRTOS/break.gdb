set pagination off
set logging file gdb.log
set logging overwrite
set logging on

set architecture arm
target extended-remote localhost:3333
monitor soft_reset_halt
file bin/Demo.elf
load bin/Demo.elf



# break _start
# break vTaskStartScheduler
# break xPortStartScheduler
# break vPortISRStartFirstTask
# break vTickISR
# break blinkRed
# break vTaskDelay
# break blinkRed
# break xTaskIncrementTick
# break vPortYieldProcessor
# break xTaskResumeAll

break main
commands
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
end

# 1
break vTaskSwitchContext
commands
    # silent
    # printf "vTaskSwitchContext called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

break prvIdleTask
commands
    # silent
    # printf "prvIdleTask called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

break vTaskDelay
commands
    # silent
    # printf "vTaskDelay called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

# 1
break vPortYieldProcessor
commands
    # silent
    # printf "vPortYieldProcessor called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

break vTickISR
commands
    # silent
    # printf "vTickISR called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

break xTaskIncrementTick
commands
    # silent
    # printf "xTaskIncrementTick called\n"
    print xTickCount
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

# 1
break blinkRed
commands
    # silent
    # printf "blinkRed called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

break toggle_led
commands
    # silent
    # printf "toggle_led called\n"
    info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
    continue
end

# break blinkGreen
# commands
#     printf "blinkGreen called\n"
#     info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
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

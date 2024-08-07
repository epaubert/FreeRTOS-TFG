set pagination off
set logging file gdb.log
set logging overwrite
# set logging redirect on
# set logging debugredirect on
set logging enabled on

set architecture arm
target extended-remote localhost:3333
monitor soft_reset_halt
file ../../bin/Demo.elf
load ../../bin/Demo.elf

# Layout
# layout split

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
info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
end

# 1
# break vTaskSwitchContext
# commands
# # silent
# # printf "vTaskSwitchContext called\n"
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break prvIdleTask
# commands
# # info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break vTaskDelay
# commands
#     # silent
#     # printf "vTaskDelay called\n"
#     # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# 1
# break vPortYieldProcessor
# commands
# # silent
# # printf "vPortYieldProcessor called\n"
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

break vTickISR
commands
# silent
# printf "vTickISR called\n"
# info registers
info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
continue
end

break xTaskIncrementTick
commands
# silent
# printf "xTaskIncrementTick called\n"
# print xTickCount
# info registers
# info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
continue
end

# 1
# break vBlinkRed
# commands
#     # silent
#     # printf "blinkRed called\n"
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break toggle_led
# commands
# # silent
# # printf "toggle_led called\n"
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break vBlinkGreen
# commands
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

break vParTestToggleLED
commands
info registers
continue
end

break vParTestInitialise
commands
# info registers lr cpsr
continue 
end

break vParTestSetLED
commands
# info registers lr cpsr
continue 
end

break vStartLEDFlashTasks
commands
# info registers lr cpsr
continue 
end

break vLEDFlashTask
commands
info registers lr cpsr
continue 
end

break vParTestSetLED
commands
# info registers lr cpsr
continue 
end

# break _soft_reset_handler
# commands
# continue
# end
# break _undef_handler
# commands
# continue
# end
# break _swi_handler
# commands
# continue
# end
# break _pabt_handler
# commands
# continue
# end
# break _dabt_handler
# commands
# continue
# end
# break _irq_handler
# commands
# continue
# end
# break _fiq_handler
# commands
# continue
# end

# break vPortEnterCritical
# commands
# # info registers
# # info registers lr lr_irq lr_svc
# continue
# end

# TODO: hacer un watch en los bits de interrupciones
# watch *(volatile uint32_t*)0xADDRESS
# commands
#     silent
#     printf "Interrupt bit changed\n"
#     continue
# end

continue

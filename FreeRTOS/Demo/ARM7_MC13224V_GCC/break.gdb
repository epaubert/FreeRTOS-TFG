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

break _start
commands
info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
continue
end

break main
commands
# info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
end

break vApplicationStackOverflowHook
commands
info registers
x/5c $r1
continue
end

break vCrash
commands
info registers
end


break excep_nonnested_fiq_handler_asm
commands
# silent
# printf "\n\nexcep_nonnested_fiq_handler_asm\n"
# info registers sp sp_usr sp_irq sp_fiq sp_svc 
info registers cpsr sp sp_fiq lr
continue
end

break itc_service_fast_interrupt
commands
# silent
# printf "itc_service_fast_interrupt\n"
# info registers sp sp_usr sp_irq sp_fiq sp_svc 
info registers cpsr sp sp_fiq lr
continue
end

break vTickISR
commands
silent
printf "vTickISR called\n"
info registers lr
# info registers lr sp sp_usr sp_irq sp_fiq sp_svc 
# info registers cpsr sp sp_fiq lr
continue
end

# break xTaskIncrementTick
# commands
# # silent
# # printf "xTaskIncrementTick called\n"
# print xTickCount
# # info registers
# # info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break clearInt
# commands
# continue
# end

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

# break vTaskDelayUntil
# commands
#     # silent
#     # printf "vTaskDelay called\n"
#     # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break vPortYieldProcessor
# commands
# # silent
# # printf "vPortYieldProcessor called\n"
# # info registers
# #info registers cpsr spsr_irq spsr_svc lr lr_irq lr_svc
# continue
# end

# break vBlinkRed
# commands
#     # silent
#     # printf "blinkRed called\n"
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

# break vParTestSetLED
# commands
# info registers r0
# continue
# end
#
# break vParTestToggleLED
# commands
# info registers lr
# # info registers r0
# continue
# end

# break vFibonacciTask
# commands
# continue
# end

# break print_int
# commands
# continue
# end

# break vParTestInitialise
# commands
# # info registers lr cpsr
# continue 
# end

# break vStartLEDFlashTasks
# commands
# # info registers lr cpsr
# continue 
# end

# break vLEDFlashTask
# commands
# info registers lr cpsr
# continue 
# end

# break vParTestSetLED
# commands
# # info registers lr cpsr
# continue 
# end

# break _soft_reset_handler
# commands
# info registers
# end

break _undef_handler
commands
info registers
# continue
end

# break _swi_handler
# commands
# continue
# end

break _pabt_handler
commands
info registers
# continue
end

break _dabt_handler
commands
info registers
# continue
end

# break _irq_handler
# commands
# continue
# end

# break itc_service_fast_interrupt
# commands
# info registers lr
# continue
# end

# break excep_nested_irq_handler
# commands
# info registers
# continue
# end

# break itc_service_normal_interrupt
# commands
# info registers lr
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

# break print_str
# commands
# info registers lr
# # info registers r0
# continue
# end

# TODO: hacer un watch en los bits de interrupciones
# watch *(volatile uint32_t*)0xADDRESS
# commands
#     silent
#     printf "Interrupt bit changed\n"
#     continue
# end

# layout split
continue

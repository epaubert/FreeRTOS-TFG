set architecture arm
target extended-remote localhost:3333
monitor soft_reset_halt
file bin/Demo.elf
load bin/Demo.elf
trace _start
break main
# trace vTaskStartScheduler
# trace xPortStartScheduler
# trace vPortISRStartFirstTask
# trace vTickISR
# trace blinkRed
# trace vTaskDelay
# trace blinkRed

# trace xTaskIncrementTick

# trace vPortYieldProcessor
# trace xTaskResumeAll

# 1
trace vTaskSwitchContext
commands
	collect $_ret
	collect $locals
end

trace prvIdleTask
commands
	collect $_ret
	collect $locals
end

trace vTaskDelay
commands
	collect $_ret
	collect $locals
end

# 1
trace vPortYieldProcessor
commands
	collect $_ret
	collect $locals
end

trace vTickISR
commands
	collect $_ret
	collect $locals
end

trace xTaskIncrementTick
commands
	collect $_ret
	collect $locals
end

# 1
trace blinkRed
commands
	collect $_ret
	collect $locals
end

trace toggle_led
commands
	collect $_ret
	collect $locals
end

# trace blinkGreen
# commands
# end

# TODO: hacer un watch en los bits de interrupciones
# watch *(volatile uint32_t*)0xADDRESS
# commands
#     printf "Interrupt bit changed\n"
#     continue
# end

# Layout
# layout split

continue

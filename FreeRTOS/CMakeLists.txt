cmake_minimum_required(VERSION "3.27.7")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BIN "/run/current-system/sw/bin/")
set(SHELL "${BIN}sh")

set(PORT_DIR "Source/portable/GCC/ARM7_MC13224V/")
set(DEMO_DIR "Demo/ARM7_MC13224V_GCC/")
set(TOOL_DIR "${DEMO_DIR}tools-econotag/")
set(BSP_DIR "${PORT_DIR}bsp/")
set(LINKER_SCRIPT "${DEMO_DIR}econotag.ld")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "arm7tdmi")
set(CROSS_COMPILE "${BIN}arm-none-eabi-")

set(CMAKE_C_COMPILER "${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE}g++")
set(CMAKE_C_LINKER "${CROSS_COMPILE}ld")
set(CMAKE_C_ASSEMBLER "${CROSS_COMPILE}as")
set(CMAKE_AR "${CROSS_COMPILE}ar")

set(CMAKE_EXE_LINKER_FLAGS "-specs=nosys.specs" CACHE INTERNAL "")

# Flags
set(CFLAGS "-c -g -Og -Wall -mcpu=arm7tdmi") # -mthumb -mthumb-interwork
set(ASFLAGS "-gstabs -mcpu=arm7tdmi -mfpu=softfpa") # -mthumb
set(ARFLAGS "-src")
set(LDFLAGS 
    -nostartfiles
    "-T${LINKER_SCRIPT}"
)

# set(SRAM_BASE "0x00400000")
set(FREERTOS_PORT "A_CUSTOM_PORT") #  GCC MC13224V

project(port_freertos_econotag C ASM)

add_subdirectory(${PORT_DIR})

set(FREERTOS_KERNEL_PATH "Source")

# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
    INTERFACE
    "${PORT_DIR}"
)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    # Adding the following configurations to build SMP template port
    add_compile_options( -DconfigNUMBER_OF_CORES=1 -DconfigUSE_PASSIVE_IDLE_HOOK=0 )
endif()

# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "1" CACHE STRING "" FORCE)

# Select the native compile PORT
# set(FREERTOS_PORT ${FREERTOS_PORT} CACHE STRING "" FORCE)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

########################################################################
# Overall Compile Options
# Note the compile option strategy is to error on everything and then
# Per library opt-out of things that are warnings/errors.
# This ensures that no matter what strategy for compilation you take, the
# builds will still occur.
#
# Only tested with GNU and Clang.
# Other options are https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID Naming of compilers translation map:
#
#   FreeRTOS    | CMake
#   -------------------
#   CCS         | ?TBD?
#   GCC         | GNU, Clang, *Clang Others?
#   IAR         | IAR
#   Keil        | ARMCC
#   MSVC        | MSVC # Note only for MinGW?
#   Renesas     | ?TBD?

target_compile_options(freertos_kernel PRIVATE
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,GNU>:-g>
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-Og>
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-mcpu=arm7tdmi >
    # $<$<COMPILE_LANG_AND_ID:C,GNU>:-mthumb>
    # $<$<COMPILE_LANG_AND_ID:C,GNU>:-mthumb-interwork >

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align> )

add_executable(Demo.elf
    ${DEMO_DIR}main.c
    # ${DEMO_DIR}ParTest/ParTest.c
    # Demo/Common/Minimal/integer.c
    # Demo/Common/Minimal/flash.c
    # Demo/Common/Minimal/BlockQ.c
)

target_include_directories( Demo.elf PRIVATE
    Demo/Common/include/
    ${BSP_DIR}/include/
)

# add_link_options("-fuse-ld=${CMAKE_C_LINKER} ${LDFLAGS}")
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})
target_link_options(Demo.elf PRIVATE 
    "${LDFLAGS}"
)

target_link_libraries(Demo.elf freertos_kernel freertos_config)

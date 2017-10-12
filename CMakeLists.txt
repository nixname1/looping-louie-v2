cmake_minimum_required(VERSION 3.6)
project(Looping_Louie_V2)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CROSS_COMPILER_PREFIX arm-none-eabi-)

#include(CMakeForceCompiler)
#CMAKE_FORCE_C_COMPILER(/usr/bin/arm-none-eabi-gcc GNU)
#CMAKE_FORCE_CXX_COMPILER(/usr/bin/arm-none-eabi-g++ GNU)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_LINK_EXECUTABLE arm-none-eabi-ld)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_VERBOSE_MAKEFILE 1)

include_directories(include system/include system/include/cmsis system/include/stm32f4-hal)

#add global CPU settings
add_definitions(-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)

#add global defines
add_definitions(-DSTM32F411xE -DDEBUG -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_HAL_DRIVER -DHSE_VALUE=8000000)

#add global flags
add_definitions(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -std=gnu11)

#add global warnings
add_definitions(-Wunused
        -Wuninitialized
        -Wall
        -Wbad-function-cast
        -Wextra
        -Wmissing-declarations
        -Wmissing-prototypes
        -Wconversion
        -Wpointer-arith
        -Wstrict-prototypes
        -Wpadded
        -Wshadow
        -Wlogical-op
        -Waggregate-return
        -Wfloat-equal)
#warnings only for gcc - not for g++
set(CMAKE_C_FLAGS_DEBUG -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast)
set(CMAKE_C_FLAGS_RELEASE -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast)

#add debug flags
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -DUSE_FULL_ASSERT -DTRACE -O0 -g3 -fno-move-loop-invariants")

#add release flags
set(CMAKE_C_FLAGS_RELEASE "-O3")

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ldscripts /usr/arm-none-eabi/lib/fpu /usr/lib/gcc/arm-none-eabi/6.2.0/fpu)

set(SOURCE_FILES
    src/_initialize_hardware.c
    src/main.c
    src/ll_lightbarrier.c
    src/ll_led.c
    src/ll_motor.c
    src/hardware/ll_system.c
    src/ll_anim.c
    src/ll_player.c
    src/ll_game.c
    src/ll_switch.c
    src/anim/system_boot.c
    src/anim/game_start.c
    src/hardware/ll_gpio.c
    src/hardware/ll_74hc166.c
    src/hardware/ll_renderer.c
    #src/stm32f4xx_hal_msp.c
    system/src/cmsis/system_stm32f4xx.c
    system/src/cmsis/vectors_stm32f411xe.c
    system/src/cortexm/_initialize_hardware.c
    system/src/cortexm/_reset_hardware.c
    system/src/cortexm/exception_handlers.c
    system/src/diag/Trace.c
    system/src/diag/trace_impl.c
    system/src/newlib/_cxx.cpp
    system/src/newlib/_exit.c
    system/src/newlib/_sbrk.c
    system/src/newlib/_startup.c
    system/src/newlib/_syscalls.c
    system/src/newlib/assert.c
    system/src/stm32f4-hal/stm32f4xx_hal.c
    #system/src/stm32f4-hal/stm32f4xx_hal_adc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_adc_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_can.c
    #system/src/stm32f4-hal/stm32f4xx_hal_cec.c
    system/src/stm32f4-hal/stm32f4xx_hal_cortex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_crc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_cryp.c
    #system/src/stm32f4-hal/stm32f4xx_hal_cryp_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dac.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dac_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dcmi.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dcmi_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dma.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dma2d.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dma_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_dsi.c
    #system/src/stm32f4-hal/stm32f4xx_hal_eth.c
    system/src/stm32f4-hal/stm32f4xx_hal_flash.c
    #system/src/stm32f4-hal/stm32f4xx_hal_flash_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_flash_ramfunc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_fmpi2c.c
    #system/src/stm32f4-hal/stm32f4xx_hal_fmpi2c_ex.c
    system/src/stm32f4-hal/stm32f4xx_hal_gpio.c
    #system/src/stm32f4-hal/stm32f4xx_hal_hash.c
    #system/src/stm32f4-hal/stm32f4xx_hal_hash_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_hcd.c
    #system/src/stm32f4-hal/stm32f4xx_hal_i2c.c
    #system/src/stm32f4-hal/stm32f4xx_hal_i2c_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_i2s.c
    #system/src/stm32f4-hal/stm32f4xx_hal_i2s_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_irda.c
    system/src/stm32f4-hal/stm32f4xx_hal_iwdg.c
    #system/src/stm32f4-hal/stm32f4xx_hal_lptim.c
    #system/src/stm32f4-hal/stm32f4xx_hal_ltdc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_ltdc_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_msp_template.c
    #system/src/stm32f4-hal/stm32f4xx_hal_nand.c
    #system/src/stm32f4-hal/stm32f4xx_hal_nor.c
    #system/src/stm32f4-hal/stm32f4xx_hal_pccard.c
    #system/src/stm32f4-hal/stm32f4xx_hal_pcd.c
    #system/src/stm32f4-hal/stm32f4xx_hal_pcd_ex.c
    system/src/stm32f4-hal/stm32f4xx_hal_pwr.c
    #system/src/stm32f4-hal/stm32f4xx_hal_pwr_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_qspi.c
    system/src/stm32f4-hal/stm32f4xx_hal_rcc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_rcc_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_rng.c
    #system/src/stm32f4-hal/stm32f4xx_hal_rtc.c
    #system/src/stm32f4-hal/stm32f4xx_hal_rtc_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_sai.c
    #system/src/stm32f4-hal/stm32f4xx_hal_sai_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_sd.c
    #system/src/stm32f4-hal/stm32f4xx_hal_sdram.c
    #system/src/stm32f4-hal/stm32f4xx_hal_smartcard.c
    #system/src/stm32f4-hal/stm32f4xx_hal_spdifrx.c
    #system/src/stm32f4-hal/stm32f4xx_hal_spi.c
    #system/src/stm32f4-hal/stm32f4xx_hal_sram.c
    #system/src/stm32f4-hal/stm32f4xx_hal_tim.c
    #system/src/stm32f4-hal/stm32f4xx_hal_tim_ex.c
    #system/src/stm32f4-hal/stm32f4xx_hal_timebase_tim_template.c
    #system/src/stm32f4-hal/stm32f4xx_hal_uart.c
    #system/src/stm32f4-hal/stm32f4xx_hal_usart.c
    #system/src/stm32f4-hal/stm32f4xx_hal_wwdg.c
    #system/src/stm32f4-hal/stm32f4xx_ll_fmc.c
    #system/src/stm32f4-hal/stm32f4xx_ll_fsmc.c
    #system/src/stm32f4-hal/stm32f4xx_ll_sdmmc.c
    #system/src/stm32f4-hal/stm32f4xx_ll_usb.c)
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-Map,${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -T libs.ld -T mem.ld -T sections.ld -Xlinker --gc-sections -nostartfiles -flto -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb --specs=nano.specs -fdata-sections -ffreestanding -ffunction-sections -flto -fmessage-length=0 -fsigned-char")
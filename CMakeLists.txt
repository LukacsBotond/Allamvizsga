# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/botond/Desktop/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(src C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(src src.cpp
        
        ADC/ADC.cpp
        ADC/ADCCorrecter.cpp
        Calculate/BaseCalculate.cpp
        Calculate/BaseValues.cpp
        Calculate/BaseCleanInput.cpp
        Calculate/ICleanInput.cpp

        #display
        display/ili9341.cpp
        display/characterDisplay.cpp
        #display/graphDisplay.cpp

        controll/spi.cpp
        controll/spiPorts.cpp
        controll/BaseSwitchController.cpp
        #controll/Aswitch.cpp
        controll/Aswitch.cpp

        common/common.cpp
        
        stateMachine/machineStates.cpp

        # test cases
        Tests/BaseCleanInputTest.cpp
        Tests/TestPrinter.cpp
        )

pico_set_program_name(src "src")
pico_set_program_version(src "0.1")

pico_enable_stdio_uart(src 0)
pico_enable_stdio_usb(src 1)

# Add the standard library to the build
target_link_libraries(src pico_stdlib)

# Add any user requested libraries
target_link_libraries(src
        hardware_spi
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_adc
        hardware_dma
        pico_multicore
 #       pico_sem
        pico_stdlib
        hardware_clocks
        hardware_pll
        )

#target_link_libraries(src
#        display/include/ili9341.h
#)

pico_add_extra_outputs(src)


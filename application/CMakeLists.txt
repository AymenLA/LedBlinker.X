cmake_minimum_required(VERSION 3.18)

# the current build is not native but cross compiled
set(CMAKE_SYSTEM_NAME Generic)

# forcing cmake to use the next compilers
set(CMAKE_C_COMPILER "/opt/microchip/xc8/v2.36/bin/xc8-cc" CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "/opt/microchip/xc8/v2.36/bin/xc8-cc" CACHE STRING "C++ compiler" FORCE)
if((NOT EXISTS ${CMAKE_C_COMPILER}) OR (NOT EXISTS ${CMAKE_CXX_COMPILER}))
    # suitable compiler for project not fount, leave with fatal error
    message(FATAL_ERROR "Microchip 8-bits PIC(R) compiler not found")
else()
    # prevent cmake from testing the compiler
    set(CMAKE_C_COMPILER_WORKS 1)
    set(CMAKE_CXX_COMPILER_WORKS 1)
    message(STATUS "Microchip 8-bits PIC(R) C/C++ Compiler found, skipping cmake test")
endif()

# target device
# @TODO if target device is empty string or not listed in compiler directory, leave with fatal error
set(MCU_DEVICE "18F4550")

# target device familly package
# @TODO dynamic family pack based on current target device
set(DEVICE_FIMILLY_PACK "/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8")
if(NOT EXISTS ${DEVICE_FIMILLY_PACK})
    # target device familly package not found, leave with fatal error
    message(FATAL_ERROR "Microchip 8-bits PIC(R) device familly package not found")
else()
    message(STATUS "Microchip 8-bits PIC(R) device PIC${MCU_DEVICE} familly package found")
endif()

# @TODO extract project name from current directory
project(BTRLEDBLINKER VERSION 0.1)

# @TODO dynamicly get application subdirectories for source files
set(FIRMWARE_DIR FirmwareApp)
set(BSP_DIR bsp)
set(CONFIGURATION_DIR configuration)

# @TODO dynamicly get application source files
set(SRC_FILES ${CMAKE_SOURCE_DIR}/main.c 
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppButtons.c
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppCustomDevice.c
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppLeds.c
              ${CMAKE_SOURCE_DIR}/${BSP_DIR}/src/Buttons.c
              ${CMAKE_SOURCE_DIR}/${BSP_DIR}/src/Leds.c
              ${CMAKE_SOURCE_DIR}/${CONFIGURATION_DIR}/src/System.c)

# @TODO dynamicly get application subdirectories for header files
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/${BSP_DIR}/include
                 ${CMAKE_SOURCE_DIR}/${CONFIGURATION_DIR}/include)


# @TODO executable name should be the same as project name
add_executable(BTRLEDBLINKER ${SRC_FILES})
target_include_directories(BTRLEDBLINKER PUBLIC ${INCLUDE_DIRS})

# @TODO more compile options are to be provided as variables
target_compile_options(BTRLEDBLINKER PUBLIC -mcpu=${MCU_DEVICE} -mdfp=\"${DEVICE_FIMILLY_PACK}\" -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto)

# @TODO resolve link error

# @TODO add burn target command
# @TODO find java, find ipecmd.jar
# command : /opt/microchip/mplabx/v6.00/sys/java/zulu8.54.0.21-ca-fx-jre8.0.292-linux_x64/bin/java -jar /opt/microchip/mplabx/v6.00/mplab_platform/mplab_ipe/ipecmd.jar -P18F4550 -TPPK4 -M -F"/home/jeanlinux/workspace/MPLABXProjects/LedBlinker.X/dist/default/production/LedBlinker.X.production.hex" -OL
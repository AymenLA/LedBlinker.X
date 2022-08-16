cmake_minimum_required(VERSION 3.18)

if(NOT EXISTS "/opt/microchip/xc8/v2.36/bin/xc8-cc")
    message(FATAL_ERROR "Microchip 8-bits PIC(R) compiler not found")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(MCU_DEVICE "18F4550")
if(NOT EXISTS "/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8")
    message(FATAL_ERROR "Microchip 8-bits PIC(R) device familly package not found")
endif()

set(DEVICE_FIMILLY_PACK "/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8")
set(CMAKE_C_COMPILER "/opt/microchip/xc8/v2.36/bin/xc8-cc" CACHE STRING "C compiler" FORCE)
set(CMAKE_C_FLAGS "${DEVICE_FIMILLY_PACK} -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits   -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto")

project(BTRLEDBLINKER VERSION 0.1)

set(FIRMWARE_DIR FirmwareApp)
set(BSP_DIR bsp)
set(CONFIGURATION_DIR configuration)

set(SRC_FILES ${CMAKE_SOURCE_DIR}/main.cpp 
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppButtons.c
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppCustomDevice.c
              ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/src/AppLeds.c
              ${CMAKE_SOURCE_DIR}/${BSP_DIR}/src/Buttons.c
              ${CMAKE_SOURCE_DIR}/${BSP_DIR}/src/Leds.c
              ${CMAKE_SOURCE_DIR}/${CONFIGURATION_DIR}/src/System.c)

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/${FIRMWARE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/${BSP_DIR}/include
                 ${CMAKE_SOURCE_DIR}/${CONFIGURATION_DIR}/include)

add_executable(BTRLEDBLINKER ${SRC_FILES})
target_include_directories(BTRLEDBLINKER PUBLIC ${INCLUDE_DIRS})
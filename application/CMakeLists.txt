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

set(YOUR_MPLABX_INSTALL_DIRECTORY "/opt/microchip/mplabx/v6.00" CACHE STRING "MPLAB X v6.00 install directory")
set(MPLABX_JAVA_EXECUTABLE_PATH "${YOUR_MPLABX_INSTALL_DIRECTORY}/sys/java/zulu8.54.0.21-ca-fx-jre8.0.292-linux_x64/bin/java" CACHE STRING "MPLAB X java executable")
set(MPLAB_IPE_CMD_COMMAND "${YOUR_MPLABX_INSTALL_DIRECTORY}/mplab_platform/mplab_ipe/ipecmd.jar" CACHE STRING "MPLAB IPE programmer command line")

if((NOT EXISTS ${MPLABX_JAVA_EXECUTABLE_PATH}) OR (NOT EXISTS ${MPLAB_IPE_CMD_COMMAND}))
    # one or more elements needed by the burn target command was not found, print a warning message
    message(WARNING "MPLAB IPE Programmer environment not found, \"make burn\" command will not work")
endif()

# target device
# @TODO if target device is empty string or not listed in compiler directory, leave with fatal error
# regex to verify MCU device name format : sed -n -E '/^1[268][fF][[:digit:]][[:digit:]][[:alpha:]]?[[:digit:]][[:digit:]]?$/p'
# starts with '1', second character is one from [2, 6, 8], third character if upper or lower case 'F', followed by 2 digits, 
# could have one letter in the middle and is always fallowed by one digit or two
set(MCU_DEVICE "18F4550")

# tool selection could be done by 
# P : programmer name
# S : serial number
set(PIC_PROGRAMMER_TOOL_SELECTION "P")

# use programmer name or serial number
# PK4 : PICKIT4
# PK3 : PICKIT3
# BURxxxxxxxxx : serial number
set(PIC_PROGRAMMER_IDENTIFICATION "PK4")

# target device familly package
# @TODO dynamic family pack based on current target device
set(DEVICE_FIMILLY_PACK "/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8")
if(NOT EXISTS ${DEVICE_FIMILLY_PACK})
    # target device familly package not found, leave with fatal error
    message(FATAL_ERROR "Microchip 8-bits PIC(R) device familly package not found")
else()
    message(STATUS "Microchip 8-bits PIC(R) device PIC${MCU_DEVICE} familly package found")
endif()

# extract project name from current directory
STRING(REGEX REPLACE "(.*\/)(.*)(\/build$)" "\\2" PROJECT_DIR_NAME ${CMAKE_CURRENT_BINARY_DIR})
# remove any special character in project name since linker doesn't accept that in the production files names
string(REGEX REPLACE "[^a-zA-Z]" "" PROJECT_DIR_NAME_CLEARED ${PROJECT_DIR_NAME})
message(STATUS "Setting project name to ${PROJECT_DIR_NAME_CLEARED}")


project(${PROJECT_DIR_NAME_CLEARED} VERSION 0.1)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_C 1)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX 1)

# specify the xml file for this build to be generated during the link stage
set(TARGET_XML_FILE_NAME "${CMAKE_PROJECT_NAME}.xml")
set(LINK_TARGET_XML_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_XML_FILE_NAME})

# specify the map file for this build to be generated during the link stage
set(TARGET_MAP_FILE_NAME "${CMAKE_PROJECT_NAME}.map")
set(LINK_TARGET_MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_MAP_FILE_NAME})

# specify the elf file for this build to be generated at the end of the link stage
# important note : this file name definition is not used, the compiler produces the elf file on its own
set(TARGET_ELF_FILE_NAME "${CMAKE_PROJECT_NAME}.elf")
set(LINK_TARGET_ELF_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ELF_FILE_NAME})

# specify the elf file for this build to be generated at the end of the link stage
set(PRODUCTION_HEX_FILE_NAME "${CMAKE_PROJECT_NAME}.hex")
set(PRODUCTION_HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCTION_HEX_FILE_NAME})

# find all source files, needed for the executable definition
# important note : xc8 compiler doesn't compile cpp files, or at least the passed parameters on the compile options
# doesn't allow it do so
file (GLOB_RECURSE MY_SRCS "*.c")

foreach(SRC_FILE_PATH IN LISTS MY_SRCS)
    # we are able of reading source files one by one, this will be usefull for compilation command line
    STRING(REGEX REPLACE "(.+\/)(.*)" "\\2" SRC_FILE_NAME ${SRC_FILE_PATH})
    # object file names are extracted here
    list(APPEND SOURCE_FILES_LIST ${SRC_FILE_PATH})
endforeach()

# find all header files, needed for the include paths definition
file (GLOB_RECURSE MY_INCLUDE_DIRS "*.h" ||"*.hpp")

set(INCLUDE_DIRS_LIST "")

foreach(INCLUDE_FILE IN LISTS MY_INCLUDE_DIRS)
    # remove header files names from include path 
    STRING(REGEX REPLACE "(.+\/)(.*)" "\\1" INCLUDE_FILE_PATH ${INCLUDE_FILE})
    list(APPEND INCLUDE_DIRS_LIST ${INCLUDE_FILE_PATH})
endforeach()

# clean include directories by removing duplicates
list(REMOVE_DUPLICATES INCLUDE_DIRS_LIST)

# executable name is the same as project name
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES_LIST})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE_DIRS_LIST})

# execute xc8 --help to find out about each compilation option
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -mcpu=${MCU_DEVICE} -mdfp=\"${DEVICE_FIMILLY_PACK}\" -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits   -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto)

# use .xml, .map and .hex files name defined above
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -mcpu=${MCU_DEVICE} -Wl,-Map=${LINK_TARGET_MAP_FILE}  -DXPRJ_default=default  -Wl,--defsym=__MPLAB_BUILD=1 -mdfp=\"${DEVICE_FIMILLY_PACK}\" -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -Wl,--memorysummary,${LINK_TARGET_XML_FILE})

# @TODO : the following command does nothing, there is an issue with executing sed from command in cmake, execute manually until solved
add_custom_target(pcode 
                    COMMAND echo \"-- Reconfiguring generated files to use p-code objects instead of gnu objects\"
                    COMMAND for line in $\(grep \-rl \"\.c.obj\" ${CMAKE_CURRENT_BINARY_DIR}\)\; do sed -i -E \'s/[.]obj/.p1/gm\' \"$line\"\; done)

# use .hex file generated after compilation
add_custom_target(burn
                    DEPENDS ${PRODUCTION_HEX_FILE}
                    DEPENDS ${MPLABX_JAVA_EXECUTABLE_PATH}
                    DEPENDS ${MPLAB_IPE_CMD_COMMAND}
                    COMMAND ${MPLABX_JAVA_EXECUTABLE_PATH} -jar ${MPLAB_IPE_CMD_COMMAND} -P${MCU_DEVICE} -T${PIC_PROGRAMMER_TOOL_SELECTION}${PIC_PROGRAMMER_IDENTIFICATION} -M -F\"${PRODUCTION_HEX_FILE}\" -OL)
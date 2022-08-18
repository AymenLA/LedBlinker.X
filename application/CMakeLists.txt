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
# regex to verify MCU device name format : sed -n -E '/^1[268][fF][[:digit:]][[:digit:]][[:alpha:]]?[[:digit:]][[:digit:]]?$/p'
# starts with '1', second character is one from [2, 6, 8], third character if upper or lower case 'F', followed by 2 digits, 
# could have one letter in the middle and is always fallowed by one digit or two
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
set(CMAKE_USER_MAKE_RULES_OVERRIDE_C 1)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX 1)

# specify the memoryfile for this build
set(TARGET_MEMORY_FILE_NAME "memoryfile.xml")
file (GLOB_RECURSE TARGET_MEMORY_FILE ${TARGET_MEMORY_FILE_NAME})
if(${TARGET_MEMORY_FILE} EQUAL "")
    message(FATAL_ERROR "Project specify memoryfile for this build was not found")
else()
    # move the memoryfile to the build directory
    # @TODO find a way to generate this file automatically
    file(COPY ${TARGET_MEMORY_FILE} 
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
    message(STATUS "Project specify memoryfile for this build moved to ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_MEMORY_FILE_NAME}")
endif()

# specify the map file for this build to be generated during the link stage
set(TARGET_MAP_FILE "project.map")
set(LINK_TARGET_MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_MAP_FILE})

# specify the elf file for this build to be generated at the end of the link stage
set(TARGET_ELF_FILE "project.elf")
set(LINK_TARGET_ELF_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ELF_FILE})

file (GLOB_RECURSE MY_SRCS "*.c")

foreach(SRC_FILE_PATH IN LISTS MY_SRCS)
    # we are able of reading source files one by one, this will be usefull for compilation command line
    STRING(REGEX REPLACE "(.+\/)(.*)" "\\2" SRC_FILE_NAME ${SRC_FILE_PATH})
    # object file names are extracted here
    list(APPEND SOURCE_FILES_LIST ${SRC_FILE_PATH})
endforeach()

file (GLOB_RECURSE MY_INCLUDE_DIRS "*.h")

set(INCLUDE_DIRS_LIST "")

foreach(INCLUDE_FILE IN LISTS MY_INCLUDE_DIRS)
    # we are able of reading include directories one by one, this will be usefull for compilation command line
    STRING(REGEX REPLACE "(.+\/)(.*)" "\\1" INCLUDE_FILE_PATH ${INCLUDE_FILE})
    list(APPEND INCLUDE_DIRS_LIST ${INCLUDE_FILE_PATH})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS_LIST)

# @TODO executable name should be the same as project name
add_executable(BTRLEDBLINKER ${SOURCE_FILES_LIST})
target_include_directories(BTRLEDBLINKER PUBLIC ${INCLUDE_DIRS_LIST})
target_compile_options(BTRLEDBLINKER PUBLIC -mcpu=${MCU_DEVICE} -mdfp=\"${DEVICE_FIMILLY_PACK}\" -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits   -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto)
target_link_options(BTRLEDBLINKER PUBLIC -mcpu=${MCU_DEVICE} -Wl,-Map=${LINK_TARGET_MAP_FILE}  -DXPRJ_default=default  -Wl,--defsym=__MPLAB_BUILD=1 -mdfp=\"${DEVICE_FIMILLY_PACK}\" -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -Wl,--memorysummary,${CMAKE_CURRENT_BINARY_DIR}/${TARGET_MEMORY_FILE_NAME})

# @TODO : the following command does nothing, there is an issue with executing sed from command in cmake, execute manually until solved
add_custom_target(pcode 
                    COMMAND echo \"-- Reconfiguring generated files to use p-code objects instead of gnu objects\"
                    COMMAND for line in $\(grep \-rl \"\.c.obj\" ${CMAKE_CURRENT_BINARY_DIR}\)\; do sed -i -E \'s/[.]obj/.p1/gm\' \"$line\"\; done)

# @TODO executable name should be the same as project name
add_custom_target(burn
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/BTRLEDBLINKER.hex
                    COMMAND /opt/microchip/mplabx/v6.00/sys/java/zulu8.54.0.21-ca-fx-jre8.0.292-linux_x64/bin/java -jar /opt/microchip/mplabx/v6.00/mplab_platform/mplab_ipe/ipecmd.jar -P18F4550 -TPPK4 -M -F\"${CMAKE_CURRENT_BINARY_DIR}/BTRLEDBLINKER.hex\" -OL)
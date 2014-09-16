cmake_minimum_required(VERSION 2.6)
option(BUILD_FORCE_32 "Set to ON to enable 32 bit compilation" OFF)


IF( ($ENV{CHAOS32}) OR (BUILD_FORCE_32) )
    MESSAGE(STATUS "Enabling 32 bit Compilation")
    set (CMAKE_C_FLAGS "-m32")
    set (CMAKE_CXX_FLAGS "-m32")
    set (CMAKE_LINK_FLAGS "-m32")
ENDIF()

if( DEFINED ENV{CHAOS_FRAMEWORK} ) 
    MESSAGE(STATUS "Using $ENV{CHAOS_FRAMEWORK}")
      set (PROJECT_SOURCE_DIR $ENV{CHAOS_FRAMEWORK})
ELSE()
    MESSAGE(FATAL_ERROR "Please set CHAOS_FRAMEWORK environment")
ENDIF()

set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/usr/local)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
     
INCLUDE_DIRECTORIES(../../ source test ${PROJECT_SOURCE_DIR}/usr/local/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/usr/local/lib)

find_library(MODBUSLIB modbus PATHS ${PROJECT_SOURCE_DIR}/usr/local/lib /usr/lib,/usr/local/lib/modbus DOC "modbus library")
if(NOT MODBUSLIB)
    message(FATAL_ERROR "libmodbus not found")
endif()

ADD_DEFINITIONS(-g -O2 -Wall)


#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###

ADD_LIBRARY(common_modbus SHARED ${modbus_src})
TARGET_LINK_LIBRARIES(common_modbus ${MODBUSLIB}  pthread boost_regex boost_system)

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src test/*.cpp)
###
ADD_EXECUTABLE(test_modbus ${modbus_test_src})

TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} common_modbus pthread boost_program_options boost_regex)

INSTALL_TARGETS(/lib  common_modbus)
INSTALL_TARGETS(/bin test_modbus)
FILE(GLOB headers "core/*.h")
INSTALL(FILES modbus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)

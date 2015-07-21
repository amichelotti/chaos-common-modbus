cmake_minimum_required(VERSION 2.6)
IF (DEFINED ENV{CHAOS_BUNDLE} )
  include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)
ELSE()
  MESSAGE(STATUS "STANDALONE, environment CHAOS_BUNDLE defined")
  IF (NOT CMAKE_INSTALL_PREFIX) 
    MESSAGE(fatal "you must add -DCMAKE_INSTALL_PREFIX=<dir>")
  ELSE()
    MESSAGE(STATUS "install prefix ${CMAKE_INSTALL_PREFIX}")
  ENDIF()
  INCLUDE_DIRECTORIES(. ../..)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()



find_library(MODBUSLIB modbus PATHS ${PROJECT_SOURCE_DIR}/usr/local/lib /usr/lib,/usr/local/lib/modbus DOC "modbus library")
if(NOT MODBUSLIB)
    message(FATAL_ERROR "libmodbus not found")
endif()

#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###
IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_regex boost_system)
ENDIF()

IF(BUILD_FORCE_STATIC)
ADD_LIBRARY(common_modbus STATIC ${modbus_src})
ELSE()
ADD_LIBRARY(common_modbus SHARED ${modbus_src})
ENDIF()



# set_target_properties(common_modbus_static PROPERTIES output_name common_modbus)
TARGET_LINK_LIBRARIES(common_modbus ${MODBUSLIB}  ${libs})

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src test/*.cpp)
###
ADD_EXECUTABLE(test_modbus ${modbus_test_src})

TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} common_modbus ${libs})

INSTALL_TARGETS(/lib  common_modbus)
INSTALL_TARGETS(/bin test_modbus)
FILE(GLOB headers "core/*.h")
INSTALL(FILES modbus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)

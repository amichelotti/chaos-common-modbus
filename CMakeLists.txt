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
  INCLUDE_DIRECTORIES(. ../.. ${CMAKE_INSTALL_PREFIX}/include)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

if(DEFINED ENV{CHAOS_CROSS_HOST})
SET(CROSS "--host=$ENV{CHAOS_CROSS_HOST}")
message(status "CROSS host configure params:${CHAOS_CROSS_HOST}")
else()
SET(CROSS "")
endif()


find_library(MODBUSLIB modbus PATHS ${PROJECT_SOURCE_DIR}/usr/local/lib ${CMAKE_INSTALL_PREFIX}/lib /usr/lib,/usr/local/lib/modbus DOC "modbus library")

if(NOT MODBUSLIB)
  message(STATUS "installing lib modbus")
  file(REMOVE_RECURSE external)
  file(MAKE_DIRECTORY external)
  message(STATUS "cloning modubus")
  execute_process( 
    COMMAND git clone https://github.com/stephane/libmodbus.git 
    WORKING_DIRECTORY external
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )
  if( modbus_err)
    message(FATAL_ERROR "cloning ${out_mod} ${modbus_err}")
  endif()

  execute_process( 
    COMMAND 	git checkout v3.0.5
    WORKING_DIRECTORY external/libmodbus
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )
  if( modbus_err)
    message(FATAL_ERROR "cloning ${out_mod} ${modbus_err}")
  endif()

  message(STATUS "configuring(1) modbus")
  execute_process( 
    COMMAND ./autogen.sh
    WORKING_DIRECTORY external/libmodbus
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )

  if( modbus_err)
    message(FATAL_ERROR "configuring ${out_mod}")
  endif()

  message(STATUS "configuring")
  execute_process( 
    COMMAND ./configure --enable-static --prefix=${CMAKE_INSTALL_PREFIX} ${CROSS}
    WORKING_DIRECTORY external/libmodbus
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )

  if( modbus_err)
    message(FATAL_ERROR "configuring ${out_mod}")
  endif()

  message(STATUS "compiling")
  execute_process( 
    COMMAND make install
    WORKING_DIRECTORY external/libmodbus
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )

  if( modbus_err)
    message(FATAL_ERROR "compiling ${out_mod}")
  endif()


else()
  message(STATUS "using ${out_mod}")
endif()


find_library(MODBUSLIB NAMES libmodbus.a PATHS ${CMAKE_INSTALL_PREFIX}/lib)
#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###
IF(FrameworkLib)
SET(libs ${FrameworkLib})
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

TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} common_modbus ${libs} -lboost_regex -lboost_program_options)

INSTALL_TARGETS(/lib  common_modbus)
INSTALL_TARGETS(/bin test_modbus)
FILE(GLOB headers "core/*.h")
INSTALL(FILES ModBus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)

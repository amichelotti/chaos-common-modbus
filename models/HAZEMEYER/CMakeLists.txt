 cmake_minimum_required(VERSION 2.8)
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
PROJECT(HZLib)


IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_regex boost_system)
ENDIF()


ADD_LIBRARY(${PROJECT_NAME} STATIC ./HazemeyerLib.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MODBUSLIB} common_modbus)

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src provalo.cpp)
###
# ADD_EXECUTABLE(test_hazemeyer ${modbus_test_src})
# TARGET_LINK_LIBRARIES(test_hazemeyer ${MODBUSLIB} common_modbus HZLib ${libs} -lboost_regex -lboost_program_options)
INSTALL_TARGETS(/lib HZLib)
# INSTALL_TARGETS(/bin test_hazemeyer)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/modbus/models/HAZEMEYER)

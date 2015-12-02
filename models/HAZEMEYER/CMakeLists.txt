 cmake_minimum_required(VERSION 2.8)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)



PROJECT(HazemeyerLib)


IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_regex boost_system)
ENDIF()


ADD_LIBRARY(HZLib SHARED ./HazemeyerLib.cpp)
TARGET_LINK_LIBRARIES(HZLib ${MODBUSLIB} common_modbus)

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src provalo.cpp)
###
ADD_EXECUTABLE(test_hazemeyer ${modbus_test_src})
TARGET_LINK_LIBRARIES(test_hazemeyer ${MODBUSLIB} common_modbus HZLib ${libs} -lboost_regex -lboost_program_options)
INSTALL_TARGETS(/lib HZLib)
INSTALL_TARGETS(/bin test_hazemeyer)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/modbus/models/HAZEMEYER)

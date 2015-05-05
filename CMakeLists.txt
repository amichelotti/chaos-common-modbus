cmake_minimum_required(VERSION 2.6)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

find_library(MODBUSLIB modbus PATHS ${PROJECT_SOURCE_DIR}/usr/local/lib /usr/lib,/usr/local/lib/modbus DOC "modbus library")
if(NOT MODBUSLIB)
    message(FATAL_ERROR "libmodbus not found")
endif()

#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###
IF(BUILD_FORCE_STATIC)
ADD_LIBRARY(common_modbus STATIC ${modbus_src})
ELSE()
ADD_LIBRARY(common_modbus SHARED ${modbus_src})
ENDIF()



# set_target_properties(common_modbus_static PROPERTIES output_name common_modbus)
TARGET_LINK_LIBRARIES(common_modbus ${MODBUSLIB}  pthread boost_regex boost_system)

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src test/*.cpp)
###
ADD_EXECUTABLE(test_modbus ${modbus_test_src})

TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} common_modbus pthread boost_program_options boost_regex boost_atomic)

INSTALL_TARGETS(/lib  common_modbus)
INSTALL_TARGETS(/bin test_modbus)
FILE(GLOB headers "core/*.h")
INSTALL(FILES modbus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)

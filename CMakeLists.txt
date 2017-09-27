cmake_minimum_required(VERSION 2.8)
project(common_modbus)
ADD_DEFINITIONS(-fPIC)

CheckConfigureBuild(modbus libmodbus:v3.0.5 "--enable-static;--with-pic" https://github.com/amichelotti)

find_library(MODBUSLIB NAMES libmodbus.a PATHS ${CMAKE_INSTALL_PREFIX}/lib)
#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###
IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_regex boost_system)
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} STATIC ${modbus_src})



# set_target_properties(common_modbus_static PROPERTIES output_name common_modbus)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MODBUSLIB} ${libs})

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src test/*.cpp)
###
ADD_EXECUTABLE(test_modbus ${modbus_test_src})

TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} ${PROJECT_NAME} ${libs} -lboost_regex -lboost_program_options)

INSTALL_TARGETS(/lib ${PROJECT_NAME})
INSTALL_TARGETS(/bin test_modbus)
FILE(GLOB headers "core/*.h")
INSTALL(FILES ModBus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)
add_subdirectory(models/HAZEMEYER/)
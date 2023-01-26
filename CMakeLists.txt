cmake_minimum_required(VERSION 2.9)
project(common_modbus)
ADD_DEFINITIONS(-fPIC)
find_package(chaos)
include(${CMAKE_INSTALL_PREFIX}/CMakeMacroUtils.txt)
find_package(Boost MODULE REQUIRED REQUIRED COMPONENTS program_options system)

  CheckConfigureBuild(modbus libmodbus:v3.0.5 "--enable-static;--with-pic" https://github.com/amichelotti)
  find_library(MODBUSLIB NAMES libmodbus.a PATHS ${CMAKE_INSTALL_PREFIX}/lib)

include_directories(../../ ${Boost_INCLUDE_DIRS})
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
#### put your libcommon_modbus LIBRARY source
### files in source
FILE(GLOB modbus_src core/*.cpp)
###
IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_system)
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} STATIC ${modbus_src})



# set_target_properties(common_modbus_static PROPERTIES output_name common_modbus)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MODBUSLIB} ${libs})

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src test/*.cpp)
###
 ADD_EXECUTABLE(test_modbus ${modbus_test_src})

 TARGET_LINK_LIBRARIES(test_modbus ${MODBUSLIB} ${PROJECT_NAME} pthread -lboost_program_options)

#INSTALL_TARGETS(/lib ${PROJECT_NAME})
INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#INSTALL_TARGETS(/bin/modbus test_modbus)
INSTALL(TARGETS test_modbus
        DESTINATION "bin/modbus"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
FILE(GLOB headers "core/*.h")
INSTALL(FILES ModBus.h DESTINATION include/common/modbus)
INSTALL(FILES ${headers} DESTINATION include/common/modbus/core)
add_subdirectory(models/HAZEMEYER/)

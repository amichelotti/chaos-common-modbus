 cmake_minimum_required(VERSION 3.0)
PROJECT(HZLib)

ADD_DEFINITIONS(-DHAZEMEYER_DEBUG)

IF(FrameworkLib)
SET(libs chaos_common ${FrameworkLib})
ELSE()
SET(libs pthread boost_regex boost_system)
ENDIF()


ADD_LIBRARY(${PROJECT_NAME} STATIC ./HazemeyerLib.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MODBUSLIB} common_modbus common_debug)

## gets all sources in test, a main function must be present
FILE(GLOB modbus_test_src provalo.cpp)
###
 ADD_EXECUTABLE(test_hazemeyer ${modbus_test_src})
 TARGET_LINK_LIBRARIES(test_hazemeyer ${MODBUSLIB} common_modbus common_debug HZLib ${libs} boost_regex boost_program_options)

INSTALL(TARGETS HZLib
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# INSTALL_TARGETS(/bin test_hazemeyer)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/modbus/models/HAZEMEYER)

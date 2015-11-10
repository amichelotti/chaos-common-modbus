 cmake_minimum_required(VERSION 2.8)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)



PROJECT(HazemeyerLib)


IF(FrameworkLib)
MESSAGE(STATUS,"Ciao" )
SET(libs chaos_common ${FrameworkLib})
ELSE()
MESSAGE(STATUS,"Ciao2" )
SET(libs pthread boost_regex boost_system)
ENDIF()


ADD_LIBRARY(HZLib SHARED ./HazemeyerLib.cpp)
TARGET_LINK_LIBRARIES(HZLib ${MODBUSLIB} common_modbus)

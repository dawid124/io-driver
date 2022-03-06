# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("io-driver" C CXX)

include(CMakeListsPrivate.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
    Production ALL
    COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    Debug ALL
    COMMAND platformio -c clion debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST} src/objects/iot/IotDevices.cpp src/objects/iot/IotDevices.h src/objects/iot/rgb/RgbwLight.cpp src/objects/iot/rgb/RgbwLight.h src/objects/iot/rgb/RgbLight.cpp src/objects/iot/rgb/RgbLight.h src/objects/iot/Device.cpp src/objects/iot/Device.h src/objects/iot/IThings.cpp src/objects/iot/IThings.h src/objects/iot/neo/NeoLight.cpp src/objects/iot/neo/NeoLight.h src/objects/comunication/Color.cpp src/objects/comunication/Color.h)

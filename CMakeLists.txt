cmake_minimum_required(VERSION 3.16)
project(AviTab VERSION 0.7.1 DESCRIPTION "AviTab X-Plane plugin")

if (NOT "$ENV{NAVIGRAPH_SECRET}" STREQUAL "")
    set(NAVIGRAPH_SECRET "$ENV{NAVIGRAPH_SECRET}" CACHE INTERNAL "Copied from environment variable")
endif()
if (NOT "$ENV{CHARTFOX_CLIENTID}" STREQUAL "")
    set(CHARTFOX_CLIENTID "$ENV{CHARTFOX_CLIENTID}" CACHE INTERNAL "Copied from environment variable")
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

add_definitions("-include ${PROJECT_BINARY_DIR}/config.h")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

include(lib/CMakeLists.txt)
include(src/CMakeLists.txt)

# use pkg-config in cmake: https://cmake.org/cmake/help/v3.0/module/FindPkgConfig.html
# more variable ref: https://blog.csdn.net/bytxl/article/details/50638001
# export PKG_CONFIG_PATH=/root/github/mesos/install/pkgconfig/:${PKG_CONFIG_PATH}

# ---- Config ----
cmake_minimum_required (VERSION 2.6)
project (Example)
include(FindPkgConfig)
pkg_search_module (PROCESS REQUIRED libprocess)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (${PROCESS_FOUND})
  message(STATUS ">>> find libprocess result=${PROCESS_FOUND}")
else (${PROCESS_FOUND})
  message(FATAL_ERROR ">>> find libprocess result=${PROCESS_FOUND}")
endif (${PROCESS_FOUND})

# ---- Build ----
include_directories("${PROCESS_INCLUDE_DIRS}")
link_directories("${PROCESS_LIBRARY_DIRS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wliteral-suffix -std=c++11 -D__STDC_FORMAT_MACROS")


# add the executable
add_executable (client example-client.cpp)
target_link_libraries (client  ${PROCESS_LIBRARIES})
add_executable (server example-server.cpp)
target_link_libraries (server  ${PROCESS_LIBRARIES})




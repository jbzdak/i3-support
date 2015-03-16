cmake_minimum_required(VERSION 2.8.11)

project(i3_support_clion)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_AUTOMOC ON)

# find_package(Qt5Widgets)

add_subdirectory(keybinder EXCLUDE_FROM_ALL)

file(GLOB SRCFILES "src/*.cc")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lX11")

set(SOURCE_FILES ${SRCFILES} main.cpp)

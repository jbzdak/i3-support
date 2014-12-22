cmake_minimum_required(VERSION 2.8.11)

project(i3_support_clion)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_AUTOMOC ON)

# find_package(Qt5Widgets)

file(GLOB SRCFILES "src/*.cc")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lX11")

set(SOURCE_FILES ${SRCFILES} main.cpp)

add_executable(i3support ${SOURCE_FILES})
# target_link_libraries(i3support Qt5::Widgets)

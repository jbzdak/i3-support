cmake_minimum_required(VERSION 2.8.11)

project(pa_control)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_AUTOMOC ON)

# find_package(Qt5Widgets)

# add_subdirectory(../keybinder EXCLUDE_FROM_ALL)

file(GLOB SRCFILES "src/*.cc")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#link_directories ( ${Boost_LIBRARY_DIRS} )

# target_link_libraries(pa-test boost_unit_test_framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lX11 -lpulse")

set(SOURCE_FILES ${SRCFILES} src/DeviceList.cpp src/MainLoop.cpp)

add_executable(pa-orig-example ${SOURCE_FILES} main.cpp)

add_executable(pa-test ${SOURCE_FILES} tests.cpp)
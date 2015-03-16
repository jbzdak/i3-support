cmake_minimum_required(VERSION 2.8.11)

project(pa_control)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_AUTOMOC ON)

# find_package(Qt5Widgets)

# add_subdirectory(../keybinder EXCLUDE_FROM_ALL)

file(GLOB SRCFILES "src/*.cc")

# find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#link_directories ( ${Boost_LIBRARY_DIRS} )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lX11 -lpulse ")

set(SOURCE_FILES ${SRCFILES} src/DeviceList.cpp src/MainLoop.cpp)

ADD_LIBRARY(boost_unit_test_framework STATIC IMPORTED)
SET_TARGET_PROPERTIES(boost_unit_test_framework PROPERTIES
    IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a)
# TARGET_LINK_LIBRARIES(mytarget A boost_unit_test_framework C)

add_executable(pa-orig-example ${SOURCE_FILES} main.cpp)

add_executable(pa-test ${SOURCE_FILES} tests.cpp)

target_link_libraries(pa-test boost_unit_test_framework)
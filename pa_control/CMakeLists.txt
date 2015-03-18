cmake_minimum_required(VERSION 2.8.11)

project(pa_control)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lX11 -lpulse ")

set(SOURCE_FILES src/DeviceList.cpp src/MainLoop.cpp tests.cpp src/PaOperation.cpp src/GetSinkListOp.cpp  src/PaSink.cpp  src/SetSinkMuteOp.cpp)

ADD_LIBRARY(boost_unit_test_framework STATIC IMPORTED)
SET_TARGET_PROPERTIES(boost_unit_test_framework PROPERTIES
    IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a)

add_executable(pa-orig-example ${SOURCE_FILES} main.cpp)

add_executable(pa-test ${SOURCE_FILES} )

target_link_libraries(pa-test boost_unit_test_framework)

cmake_minimum_required(VERSION 2.8.9)
project(asio-test1)

# C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Boost libraries
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.62.0 REQUIRED system)

# Always compile in release
set(CMAKE_BUILD_TYPE Release)

# Set the output to be in bin or lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Warnings to errors
#add_definitions(-Wall -Werror -Wdeprecated-declarations)
# -Wdeprecated-declarations added to suppress asio deprecation error
add_definitions(-Wall -Werror -Wdeprecated-declarations)

include_directories(${Boost_INCLUDE_DIR})

# ECHO SERVER

file(GLOB ECHO_SERVER_SOURCES "echo-server/*.cpp")

add_executable(asio-test1-echo-server ${ECHO_SERVER_SOURCES})
target_link_libraries(asio-test1-echo-server ${Boost_LIBRARIES})

# BLOCKING ECHO CLIENT

file(GLOB BLOCKING_ECHO_CLIENT_SOURCES "blocking-echo-client/*.cpp")

add_executable(asio-test1-blocking-echo-client ${BLOCKING_ECHO_CLIENT_SOURCES})
target_link_libraries(asio-test1-blocking-echo-client ${Boost_LIBRARIES})

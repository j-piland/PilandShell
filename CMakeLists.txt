cmake_minimum_required (VERSION 2.8)

project (shell)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -lthread -Wall -Wextra -g")

file(GLOB SRCS *cpp *h)

add_executable(Shell ${SRCS})

set(EXECUTABLE_OUTPUT_PATH "./bin")



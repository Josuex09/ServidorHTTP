cmake_minimum_required(VERSION 3.2)
project(ServidorHTTP)

set (CMAKE_C_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)
add_executable(ServidorHTTP ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.2)
project(ServidorHTTP)

set (CMAKE_C_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)
add_executable(ServidorHTTP ${SOURCE_FILES} consulta/procesarConsulta.h consulta/procesarConsulta.c modo/fifo.h modo/fifo.c modo/forked.h modo/forked.c modo/threaded.h modo/threaded.c modo/pre-forked.h modo/pre-forked.c modo/pre-threaded.h modo/pre-threaded.c)
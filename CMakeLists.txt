cmake_minimum_required(VERSION 3.20)
project(cross_fox C)

set(CMAKE_C_STANDARD 99)

include_directories(fox/headers/helpers fox/headers/models/networking)

add_executable(cross_fox demo/main.c fox/src/helpers/utils.c fox/headers/helpers/utils.h fox/headers/helpers/constants.h fox.h fox/src/models/networking/server.c fox/headers/models/networking/server.h)

cmake_minimum_required(VERSION 3.20)
project(rabbit C)

set(CMAKE_C_STANDARD 99)

include_directories(rabbit/headers/helpers rabbit/headers/models/networking)

add_executable(rabbit demo/main.c rabbit/src/helpers/utils.c rabbit/headers/helpers/utils.h rabbit/headers/helpers/constants.h fox.h rabbit/src/models/networking/server.c rabbit/headers/models/networking/server.h)

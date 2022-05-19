cmake_minimum_required(VERSION 3.20)
project(rabbit C)

set(CMAKE_C_STANDARD 99)

add_executable(rabbit demo/main.c rabbit/src/utils.c rabbit/headers/utils.h rabbit/headers/constants.h fox.h rabbit/src/server.c rabbit/headers/server.h rabbit/src/hashtable.c rabbit/headers/hashtable.h rabbit/headers/defaults.h rabbit/src/errors.c rabbit/headers/errors.h rabbit/src/actions.c rabbit/headers/actions.h rabbit/headers/include.h rabbit/src/arg_array.c rabbit/headers/arg_array.h)

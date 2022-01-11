cmake_minimum_required(VERSION 3.20)
project(FondamentInfo2021 C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.c carpark.c tramline.c)
set( CMAKE_BUILD_TYPE Debug )

add_executable(FondamentInfo2021 ${SOURCE_FILES})

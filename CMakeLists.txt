cmake_minimum_required(VERSION 3.16)
project(http C)

set(CMAKE_C_STANDARD 99)


add_executable(http main.c server.c)


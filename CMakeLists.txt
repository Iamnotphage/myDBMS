cmake_minimum_required(VERSION 3.25)
project(myDBMS)

set(CMAKE_CXX_STANDARD 11)

add_executable(myDBMS main.cpp shell/shell.cpp shell/shell.h)


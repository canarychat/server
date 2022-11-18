cmake_minimum_required(VERSION 3.23)
project(network_programming)

set(CMAKE_CXX_STANDARD 20)

add_executable(network_programming main.cpp epoll.h)

cmake_minimum_required(VERSION 3.24)
project(chatroom)

set(CMAKE_CXX_STANDARD 17)

# 导入libevent
find_package(Libevent REQUIRED)
set(Libevent_LIBS event)
add_executable(chatroom main.cpp callbacks.h users.h message.h)
target_link_libraries(${PROJECT_NAME} ${Libevent_LIBS})

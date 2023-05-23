cmake_minimum_required(VERSION 3.24)
project(chatroom_server VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# 导入libevent
find_package(Libevent REQUIRED)

set(Libevent_LIBS event)
add_executable(chatroom main.cpp callbacks.h users.h message.h)
target_include_directories(chatroom PRIVATE ${Libevent_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Libevent_LIBS})

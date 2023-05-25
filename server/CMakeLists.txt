cmake_minimum_required(VERSION 3.25)
project(chatroom_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON)
add_executable(server main.cpp)
target_link_libraries(server Poco::Foundation Poco::Util Poco::Net Poco::XML Poco::JSON)

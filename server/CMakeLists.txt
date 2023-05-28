cmake_minimum_required(VERSION 3.25)
project(chatroom_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON)
AUX_SOURCE_DIRECTORY(./net NET_SRC)
AUX_SOURCE_DIRECTORY(./data DATA_SRC)
INCLUDE_DIRECTORIES(./net ./data)
add_executable(server main.cpp ${NET_SRC} ${DATA_SRC})
target_link_libraries(server Poco::Foundation Poco::Util Poco::Net Poco::XML Poco::JSON)

cmake_minimum_required(VERSION 3.1)
project(chatroom_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON ActiveRecord DataMySQL)
AUX_SOURCE_DIRECTORY(. SRC)
AUX_SOURCE_DIRECTORY(./net NET_SRC)
AUX_SOURCE_DIRECTORY(./data DATA_SRC)
AUX_SOURCE_DIRECTORY(./message MSG_SRC)
AUX_SOURCE_DIRECTORY(./support SUP_SRC)

add_subdirectory(./activeRecordModel)  # Add the activerecordModel subdirectory

INCLUDE_DIRECTORIES(./net ./data ./activeRecordModel/include ./support ./message)
add_executable(server ${SRC} ${NET_SRC} ${DATA_SRC} ${SUP_SRC} ${MSG_SRC})
target_link_libraries(server Poco::Foundation Poco::Util Poco::Net Poco::XML Poco::JSON Poco::ActiveRecord PocoUtil PocoNet PocoXML PocoJSON PocoFoundation PocoData PocoDataMySQL PocoCrypto PocoJWT PocoRedis)
target_link_libraries(server activeRecordModel)
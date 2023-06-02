# activerecordModel/CMakeLists.txt
set(ACTIVE_RECORD_MODEL_HEADERS
        include/ChatRoomDB/Message.h
        include/ChatRoomDB/Room.h
        include/ChatRoomDB/User.h
        include/ChatRoomDB/UserRoomRelation.h
        )

set(ACTIVE_RECORD_MODEL_SOURCES
        src/Message.cpp
        src/Room.cpp
        src/User.cpp
        src/UserRoomRelation.cpp
        )

# Create a library from the sources and headers.
add_library(activeRecordModel ${ACTIVE_RECORD_MODEL_SOURCES} ${ACTIVE_RECORD_MODEL_HEADERS})

# Set the include directories.
target_include_directories(activeRecordModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link the Poco libraries.
target_link_libraries(activeRecordModel PUBLIC Poco::Foundation Poco::Util Poco::Net Poco::XML Poco::JSON Poco::ActiveRecord PocoUtil PocoNet PocoXML PocoJSON PocoFoundation PocoData PocoDataMySQL)

//
// Created by lambert on 23-6-4.
//

#include "RoomManager.h"
#include "ChatRoomDB/User.h"
#include "DataFacade.h"


void RoomManager::initialize(Application &app) {
    poco_information(app.logger(), "RoomManager init");
}

Poco::JSON::Object::Ptr RoomManager::getRoomList(int user_id) {
    Poco::JSON::Array::Ptr roomList = new Poco::JSON::Array;
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);
        auto p_user = ChatRoomDB::User::find(p_context, user_id);
        if (p_user) {
            int temp_user_id = user_id;
            Poco::Data::Statement select(session);
            select << "SELECT rooms.id as roomid, rooms.name as name "
                   << "FROM rooms "
                   << "JOIN user_room_relation "
                   << "ON rooms.id = user_room_relation.room_id "
                   << "WHERE user_room_relation.user_id = ?",
                Poco::Data::Keywords::use(temp_user_id), Poco::Data::Keywords::now;

            std::string room_name;
            int room_id;
            select.execute();
            Poco::Data::RecordSet rs(select);

            for (Poco::Data::RecordSet::Iterator it = rs.begin(); it != rs.end(); ++it) {
                Poco::JSON::Object::Ptr roomObject = new Poco::JSON::Object;
                roomObject->set("roomid", (*it)["roomid"]);
                roomObject->set("name", (*it)["name"]);
                roomList->add(roomObject);
            }
        }
    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
    }

    Poco::JSON::Object::Ptr dataObject = new Poco::JSON::Object;
    dataObject->set("chatrooms", roomList);
    result->set("data", dataObject);
    return result;
}


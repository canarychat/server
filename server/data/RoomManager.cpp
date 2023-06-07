//
// Created by lambert on 23-6-4.
//

#include "RoomManager.h"
#include "ChatRoomDB/User.h"
#include "DataFacade.h"
#include "ChatRoomDB/Room.h"
#include "ChatRoomDB/UserRoomRelation.h"

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
            select << "SELECT rooms.id as roomid, rooms.name as name, "
                      "rooms.description as description , rooms.owner_id as owner_id "
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
                roomObject->set("owner_id", (*it)["owner_id"]);
                if (!(*it)["description"].isEmpty()) {
                    roomObject->set("description", (*it)["description"]);
                }
                roomList->add(roomObject);
            }
        }
    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
    }

    Poco::JSON::Object::Ptr dataObject = new Poco::JSON::Object;
    dataObject->set("chatrooms", roomList);
    result->set("data", dataObject);
    result->set("code", static_cast<int>(state_code::SUCCESS));
    result->set("msg", "获取房间列表成功");
    return result;
}
Poco::JSON::Object::Ptr RoomManager::createRoom(int owner_id, std::string room_name, std::string room_description) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    if (room_name.empty() || room_description.empty()) {
        result->set("code", static_cast<int>(state_code::CHATROOM_CLIENT_ERROR));
        result->set("msg", "房间名/描述 不能为空");
        return result;
    }

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);
        auto p_user = ChatRoomDB::User::find(p_context, owner_id);
        if (p_user) {
            ChatRoomDB::Room::Ptr room = new ChatRoomDB::Room;
            room->
                    name(room_name)
                .
                    description(room_description)
                .
                    owner_idID(owner_id);
            room->
                create(p_context);

            ChatRoomDB::UserRoomRelation::Ptr userRoomRelation = new ChatRoomDB::UserRoomRelation;
            userRoomRelation->
                    user_id(p_user)
                .
                    room_id(room);
            userRoomRelation->
                create(p_context);

            result->set("code", static_cast
                <int>(state_code::SUCCESS)
            );
            result->set("msg", "创建房间成功");
        } else {
            result->set("code", static_cast
                <int>(state_code::CHATROOM_CLIENT_ERROR)
            );
            result->set("msg", "用户不存在");
            return
                result;
        }
    } catch (
        Poco::Exception &e
    ) {
        poco_error(Application::instance().logger(), e.displayText());
        result->set("code", static_cast
            <int>(state_code::CHATROOM_SERVER_ERROR)
        );
        result->set("msg", "服务器错误");
        return
            result;
    }
    return
        result;
}
Poco::JSON::Object::Ptr RoomManager::deleteRoom(int room_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

}


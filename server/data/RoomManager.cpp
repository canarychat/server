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
Poco::JSON::Object::Ptr RoomManager::getRoomMemberList(int room_id, int user_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        // Check if the user is a member of the room
        Poco::Data::Statement checkUser(session);
        checkUser << "SELECT COUNT(*) FROM user_room_relation WHERE user_id = ? AND room_id = ?",
            Poco::Data::Keywords::use(user_id), Poco::Data::Keywords::use(room_id),
            Poco::Data::Keywords::now;
        if (checkUser.execute() == 0) {
            result->set("code", static_cast<int>(state_code::CHATROOM_CLIENT_ERROR));
            result->set("msg", "User is not a member of the room");
            return result;
        }

        // Get the list of room members
        Poco::Data::Statement select(session);
        select << "SELECT users.id as userid, users.username as username FROM users "
               << "INNER JOIN user_room_relation "
               << "ON users.id = user_room_relation.user_id "
               << "WHERE user_room_relation.room_id = ?",
            Poco::Data::Keywords::use(room_id), Poco::Data::Keywords::now;

        Poco::JSON::Array::Ptr membersList = new Poco::JSON::Array;
        select.execute();
        Poco::Data::RecordSet rs(select);

        for (Poco::Data::RecordSet::Iterator it = rs.begin(); it != rs.end(); ++it) {
            Poco::JSON::Object::Ptr memberObject = new Poco::JSON::Object;
            memberObject->set("id", (*it)["userid"]);
            memberObject->set("username", (*it)["username"]);
            membersList->add(memberObject);
        }

        // Set the result data
        Poco::JSON::Object::Ptr dataObject = new Poco::JSON::Object;
        dataObject->set("members", membersList);
        result->set("data", dataObject);
        result->set("code", static_cast<int>(state_code::SUCCESS));
        result->set("msg", "获取房间成员列表成功");

    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        result->set("code", static_cast<int>(state_code::CHATROOM_SERVER_ERROR));
        result->set("msg", "服务器错误");
    }

    return result;
}



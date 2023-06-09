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
        ChatRoomDB::User::Ptr p_user = ChatRoomDB::User::find(p_context, user_id);
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
        ChatRoomDB::User::Ptr p_user = ChatRoomDB::User::find(p_context, owner_id);
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

Poco::JSON::Object::Ptr RoomManager::getRoomMemberList(int room_id, int user_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        if (!check_exit(result, p_context, room_id, user_id)) {
            return result;
        }

        // Check if the user is a member of the room
        Poco::Data::Statement checkUser(session);
        checkUser << "SELECT 1 FROM user_room_relation WHERE user_id = ? AND room_id = ?",
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
Poco::JSON::Object::Ptr RoomManager::joinRoom(int room_id, int user_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        ChatRoomDB::Room::Ptr p_room = ChatRoomDB::Room::find(p_context, room_id);
        if (!p_room) {
            result->set("code", static_cast<int>(state_code::CHATROOM_NOT_EXIST));
            result->set("msg", "房间不存在");
            return result;
        }

        ChatRoomDB::User::Ptr p_user = ChatRoomDB::User::find(p_context, user_id);
        if (!p_user) {
            result->set("code", static_cast<int>(state_code::CHATROOM_USER_NOT_EXIST));
            result->set("msg", "用户不存在");
            return result;
        }

        Poco::Data::Statement checkUser(session);
        checkUser << "SELECT 1 FROM user_room_relation WHERE user_id = ? AND room_id = ?",
            Poco::Data::Keywords::use(user_id), Poco::Data::Keywords::use(room_id),
            Poco::Data::Keywords::now;

        if (checkUser.execute() != 0) {
            result->set("code", static_cast<int>(state_code::CHATROOM_ADDUSER_FAILED_USER_ALREADY_IN));
            result->set("msg", "用户已在房间内");
            return result;
        }

        ChatRoomDB::UserRoomRelation::Ptr user_room_relation = new ChatRoomDB::UserRoomRelation;
        user_room_relation->user_id(p_user);
        user_room_relation->room_id(p_room);
        user_room_relation->create(p_context);

        result->set("code", static_cast<int>(state_code::SUCCESS));
        result->set("msg", "加入房间成功");

    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        result->set("code", static_cast<int>(state_code::CHATROOM_SERVER_ERROR));
        result->set("msg", "服务器错误");
    }
    return result;
}
Poco::JSON::Object::Ptr RoomManager::leaveRoom(int room_id, int user_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        if (!check_exit(result, p_context, room_id, user_id)) {
            return result;
        }

        Poco::ActiveRecord::Query<ChatRoomDB::UserRoomRelation> query(p_context);
        auto rs = query.where("user_id = ? AND room_id = ?").bind(user_id).bind(room_id).execute();
        if (rs.empty()) {
            result->set("code", static_cast<int>(state_code::CHATROOM_DELETEUSER_FAILED_USER_NOT_IN));
            result->set("msg", "用户不在房间内");
            return result;
        } else {
            rs[0]->remove();
            //如果房间内没有人了，删除房间，虽然目前不可能出现，因为房主不能退出房间
            Poco::ActiveRecord::Query<ChatRoomDB::UserRoomRelation> delete_query(p_context);
            auto exist_rs= delete_query.where("room_id = ?").bind(room_id).execute();
            if (exist_rs.empty()) {
                ChatRoomDB::Room::Ptr room = ChatRoomDB::Room::find(p_context, room_id);
                room->remove();
            }
            result->set("code", static_cast<int>(state_code::SUCCESS));
            result->set("msg", "离开房间成功");
        }

    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        result->set("code", static_cast<int>(state_code::CHATROOM_SERVER_ERROR));
        result->set("msg", "服务器错误");
    }
    return result;
}
Poco::JSON::Object::Ptr RoomManager::deleteRoom(int room_id, int user_id) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;

    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        if (!check_exit(result, p_context, room_id, user_id)) {
            return result;
        }

        ChatRoomDB::Room::Ptr p_room = ChatRoomDB::Room::find(p_context, room_id);

        if (p_room->owner_idID() != user_id) {
            result->set("code", static_cast<int>(state_code::CHATROOM_DELETE_NOT_OWNER));
            result->set("msg", "用户不是房间所有者");
            return result;
        }

        // Delete all user-room relations for this room
        Poco::Data::Statement deleteRelations(session);
        deleteRelations << "DELETE FROM user_room_relation WHERE room_id = ?",
            Poco::Data::Keywords::use(room_id), Poco::Data::Keywords::now;
        deleteRelations.execute();

        // Delete the room
        p_room->remove();

        result->set("code", static_cast<int>(state_code::SUCCESS));
        result->set("msg", "删除房间成功");

    } catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        result->set("code", static_cast<int>(state_code::CHATROOM_SERVER_ERROR));
        result->set("msg", "服务器错误");
    }

    return result;
}
bool RoomManager::check_exit(Poco::JSON::Object::Ptr result,
                             Poco::ActiveRecord::Context::Ptr p_context,
                             int room_id = 0,
                             int user_id = 0) {
    if (room_id != 0) {
        ChatRoomDB::Room::Ptr p_room = ChatRoomDB::Room::find(p_context, room_id);
        if (!p_room) {
            result->set("code", static_cast<int>(state_code::CHATROOM_NOT_EXIST));
            result->set("msg", "房间不存在");
            return false;
        }
    }

    if (user_id != 0) {
        ChatRoomDB::User::Ptr p_user = ChatRoomDB::User::find(p_context, user_id);
        if (!p_user) {
            result->set("code", static_cast<int>(state_code::CHATROOM_USER_NOT_EXIST));
            result->set("msg", "用户不存在");
            return false;
        }
    }

    return true;
}





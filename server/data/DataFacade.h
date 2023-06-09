//
// Created by lambert on 23-6-4.
//

#pragma once
#include <utility>

#include "poco_headers.h"
#include "DataManager.h"
#include "UserManager.h"
#include "RoomManager.h"

struct DataFacade {
/// \brief 一个数据操作接口
    static HttpParamJSON registerUser(const std::string &username,
                                                const std::string &password,
                                                const std::string &email = "") {
        return Application::instance().getSubsystem<UserManager>().registerUser(username, password, email);
    }

    static HttpParamJSON loginUser(const std::string &username,
                                             const int &user_id,
                                             const std::string &password) {
        return Application::instance().getSubsystem<UserManager>().loginUser(username, user_id, password);
    }

    static Poco::JSON::Object::Ptr getRoomList(int user_id) {
        return Application::instance().getSubsystem<RoomManager>().getRoomList(user_id);
    }

    static Poco::Data::Session getSession() {
        return Application::instance().getSubsystem<DataManager>().getSession();
    }

    static int get_id_from_name(std::basic_string<char> username) {
        auto session = getSession();
        Poco::Data::Statement select(session);

        int id = 0;
        select << "SELECT id FROM users WHERE username = ?",
            Poco::Data::Keywords::into(id),
            Poco::Data::Keywords::use(username),
            Poco::Data::Keywords::now;
        return id;
    }

    static Poco::JSON::Object::Ptr createRoom(int owner_id, std::string room_name, std::string room_description) {
        return Application::instance().getSubsystem<RoomManager>().createRoom(owner_id, std::move(room_name), std::move(room_description));
    }

    static Poco::JSON::Object::Ptr deleteRoom(int room_id,int user_id) {
        return Application::instance().getSubsystem<RoomManager>().deleteRoom(room_id,user_id);
    }

    static Poco::JSON::Object::Ptr getRoomInfo(int room_id) {
        return Application::instance().getSubsystem<RoomManager>().getRoomInfo(room_id);
    }

    static Poco::JSON::Object::Ptr getRoomMemberList(int room_id,int user_id) {
        return Application::instance().getSubsystem<RoomManager>().getRoomMemberList(room_id,user_id);
    }

    static Poco::JSON::Object::Ptr joinRoom(int room_id,int user_id) {
        return Application::instance().getSubsystem<RoomManager>().joinRoom(room_id,user_id);
    }

    static Poco::JSON::Object::Ptr leaveRoom(int room_id,int user_id) {
        return Application::instance().getSubsystem<RoomManager>().leaveRoom(room_id,user_id);
    }

    static bool insertMsg(int room_id,int user_id,string msg){
        //借用一下UserManager,现在没必要为了它再开一个子系统
        return Application::instance().getSubsystem<UserManager>().insertMsg(user_id,room_id,std::move(msg));
    }

};// namespace DataFacade

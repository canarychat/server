//
// Created by lambert on 23-6-4.
//

#pragma once
#include "poco_headers.h"
#include "DataManager.h"
#include "UserManager.h"
#include "RoomManager.h"

struct DataFacade {
/// \brief 一个数据操作接口
    static Poco::JSON::Object::Ptr registerUser(const std::string &username,
                                                const std::string &password,
                                                const std::string &email = "") {
        return Application::instance().getSubsystem<UserManager>().registerUser(username, password, email);
    }

    static Poco::JSON::Object::Ptr loginUser(const std::string &username,
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
};// namespace DataFacade

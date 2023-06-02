//
// Created by lambert on 23-5-27.
//

#include "DataManager.h"

#include "poco_headers.h"
#include "ChatRoomDB/User.h"

void DataManager::initialize(Application &app) {
    poco_information(app.logger(), "DataManager init");
    try {
        Poco::Data::MySQL::Connector::registerConnector();
        auto data_config = app.config().getString("Data.mysql.config");
        p_session_ = std::make_unique<Poco::Data::Session>("MySQL", data_config);
        p_context_ = new Poco::ActiveRecord::Context(*p_session_);
    }
    catch (Poco::Exception &e) {
        poco_error(app.logger(), e.displayText());
        exit(1);
    }
}

Poco::JSON::Object::Ptr DataManager::registerUser(const std::string &username,
                                                  const std::string &password,
                                                  const std::string &email) {
    auto p_user = new ChatRoomDB::User;
    Poco::JSON::Object::Ptr respond_json = new Poco::JSON::Object;
    try {
        p_user->username(username).password(password).email(email);
        p_user->create(p_context_);
    }
    catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        respond_json->set("code", static_cast<int>(state_code::REGISTRATION_SERVER_ERROR));
    }
    Poco::JSON::Object::Ptr data_json = new Poco::JSON::Object;
    poco_trace_f3 (Application::instance().logger(), "id = %d, username = %s, email = %s", p_user->id(), p_user->username(), p_user->email());
    data_json->set("id", p_user->id());
    data_json->set("username", p_user->username());
    data_json->set("email", p_user->email());
    respond_json->set("data", data_json);
    respond_json->set("code", static_cast<int>(state_code::SUCCESS));
    respond_json->set("msg", "注册成功");

    return respond_json;
}



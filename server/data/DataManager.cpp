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
    //Active Record
    auto p_user = new ChatRoomDB::User;
    //Respond JSON
    Poco::JSON::Object::Ptr respond_json = new Poco::JSON::Object;

    //generate salt
    std::string salt;
    Poco::RandomInputStream rng;
    for (size_t i = 0; i < 24; ++i) {
        char c;
        rng >> c;
        salt += c;
    }
    std::stringstream ss;
    Poco::Base64Encoder base64Encoder(ss);
    base64Encoder << salt;
    base64Encoder.close();  // 关闭编码器以刷新任何缓冲的数据
    salt = ss.str();

    Poco::Crypto::DigestEngine engine("SHA256");
    engine.update(password + salt);
    std::string hashed_password = Poco::DigestEngine::digestToHex(engine.digest());
    try {
        Poco::ActiveRecord::Query<ChatRoomDB::User> query(p_context_);
        auto existing_user = query.where("username=?").bind(username).limit(1).execute();
        if (!existing_user.empty()) {
            // The username already exists. Return an error.
            poco_information(Application::instance().logger(), "Username already exists");
            respond_json->set("code", static_cast<int>(state_code::REGISTRATION_NICKNAME_EXISTS));
            return respond_json;
        }

        if (!email.empty()) {
            auto existing_email = query.where("email=?").bind(email).limit(1).execute();
            if (!existing_email.empty()) {
                // The email already exists. Return an error.
                poco_information(Application::instance().logger(), "Email already exists");
                respond_json->set("code", static_cast<int>(state_code::REGISTRATION_EMAIL_EXISTS));
                return respond_json;
            }
            p_user->email(email);
        } else {
            //set to null
        }

        p_user->username(username).password(hashed_password).salt(salt);
        p_user->create(p_context_);
    }
    catch (Poco::Exception &e) {
        // If any other exception occurs, log it and return a server error.
        poco_error(Application::instance().logger(), e.displayText());
        respond_json->set("code", static_cast<int>(state_code::REGISTRATION_SERVER_ERROR));
        return respond_json;
    }

    Poco::JSON::Object::Ptr data_json = new Poco::JSON::Object;
    poco_trace_f3 (Application::instance().logger(), "id = %d, username = %s, email = %s", p_user->id(),
                   p_user->username(), p_user->email());
    data_json->set("id", p_user->id());
    data_json->set("username", p_user->username());
    if (email.empty())
        data_json->set("email", "");
    else
        data_json->set("email", p_user->email());
    respond_json->set("data", data_json);
    respond_json->set("code", static_cast<int>(state_code::SUCCESS));
    respond_json->set("msg", "注册成功");

    return respond_json;
}

Poco::JSON::Object::Ptr
DataManager::loginUser(const std::string &username, const int &user_id, const std::string &password) {
    Poco::JSON::Object::Ptr respond_json = new Poco::JSON::Object;
    if (username.empty() && user_id == 0) {
        respond_json->set("code", static_cast<int>(state_code::LOGIN_CLIENT_ERROR));
        return respond_json;
    } else if (username.empty()) {
        auto user = ChatRoomDB::User::find(p_context_, user_id);
        if (user == nullptr) {
            respond_json->set("code", static_cast<int>(state_code::LOGIN_ID_NOT_EXIST));
            return respond_json;
        }
        string stored_salt = user->salt();
        string stored_password = user->password();
        Poco::Crypto::DigestEngine engine("SHA256");
        engine.update(password + stored_salt);
        string provided_hashed_password = Poco::DigestEngine::digestToHex(engine.digest());
        if (provided_hashed_password == stored_password) {
            Poco::JSON::Object::Ptr data_json = new Poco::JSON::Object;
            data_json->set("id", user->id());
            data_json->set("username", user->username());
            data_json->set("email", user->email());
            respond_json->set("data", data_json);
            respond_json->set("code", static_cast<int>(state_code::SUCCESS));
            respond_json->set("msg", "登录成功");
            return respond_json;
        } else {
            respond_json->set("code", static_cast<int>(state_code::LOGIN_PASSWORD_ERROR));
            return respond_json;
        }
    }

}



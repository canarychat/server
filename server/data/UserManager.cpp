//
// Created by lambert on 23-6-3.
//

#include "UserManager.h"
#include "ChatRoomDB/User.h"
#include "state_code.h"
#include "DataFacade.h"
#include "ChatRoomDB/Room.h"
#include "ChatRoomDB/Message.h"

void UserManager::initialize(Poco::Util::Application &app) {
    app.logger().information("UserManager Init");
}

HttpParamJSON UserManager::registerUser
    (const std::string &username, const std::string &password, const std::string &email) {
    //Active Record
    ChatRoomDB::User::Ptr p_user = new ChatRoomDB::User;
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
        auto session = DataFacade::getSession();

        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);

        Poco::ActiveRecord::Query<ChatRoomDB::User> query(p_context);
        auto existing_user = query.where("username=?").bind(username).limit(1).execute();
        if (!existing_user.empty()) {
            // The username already exists. Return an error.
            poco_information(Application::instance().logger(), "Username already exists");
            respond_json->set("code", static_cast<int>(state_code::REGISTRATION_NICKNAME_EXISTS));
            respond_json->set("msg", "用户名已存在");
            return HttpParamJSON(HTTPStatus::HTTP_CONFLICT, respond_json);
        }

        if (!email.empty()) {
            auto existing_email = query.where("email=?").bind(email).limit(1).execute();
            if (!existing_email.empty()) {
                // The email already exists. Return an error.
                poco_information(Application::instance().logger(), "Email already exists");
                respond_json->set("code", static_cast<int>(state_code::REGISTRATION_EMAIL_EXISTS));
                respond_json->set("msg", "邮箱已存在");
                return HttpParamJSON(HTTPStatus::HTTP_CONFLICT, respond_json);
            }
            p_user->email(email);
        } else {
            //set to null
        }

        p_user->username(username).password(hashed_password).salt(salt);
        p_user->create(p_context);
    }
    catch (Poco::Exception &e) {
        // If any other exception occurs, log it and return a server error.
        poco_error(Application::instance().logger(), e.displayText());
        respond_json->set("code", static_cast<int>(state_code::USER_SERVER_ERROR));
        respond_json->set("msg", "服务器错误");
        return HttpParamJSON(HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, respond_json);
    }

    Poco::JSON::Object::Ptr data_json = new Poco::JSON::Object;
    poco_trace_f3 (Application::instance().logger(), "id = %d, username = %s, email = %s", p_user->id(),
                   p_user->username(), p_user->email());
    data_json->set("id", p_user->id());
    data_json->set("username", p_user->username());
    respond_json->set("data", data_json);
    respond_json->set("code", static_cast<int>(state_code::SUCCESS));
    respond_json->set("msg", "注册成功");

    return HttpParamJSON(HTTPStatus::HTTP_OK, respond_json);
}

HttpParamJSON UserManager::loginUser
    (const std::string &username_, const int &user_id_, const std::string &password) {
    Poco::JSON::Object::Ptr respond_json = new Poco::JSON::Object;
    string stored_salt;
    string stored_password;
    string username(username_);
    int user_id = user_id_;

    try {
        if (username.empty() && user_id == 0) {
            respond_json->set("code", static_cast<int>(state_code::LOGIN_CLIENT_ERROR));
            respond_json->set("msg", "客户端错误");
            return HttpParamJSON(HTTPStatus::HTTP_BAD_REQUEST, respond_json);
        } else if (username.empty()) {
            //login with ID
            auto session = DataFacade::getSession();
            Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);
            ChatRoomDB::User::Ptr user = ChatRoomDB::User::find(p_context, user_id);
            if (user == nullptr) {
                respond_json->set("code", static_cast<int>(state_code::LOGIN_ID_NOT_EXIST));
                respond_json->set("msg", "ID不存在");
                return HttpParamJSON(HTTPStatus::HTTP_NOT_FOUND, respond_json);
            }
            stored_salt = user->salt();
            stored_password = user->password();
            username = user->username();
        } else if (user_id == 0) {
            //login with username
            auto session = DataFacade::getSession();
            Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);
            Poco::ActiveRecord::Query<ChatRoomDB::User> query(p_context);
            auto user = query.where("username=?").bind(username).limit(1).execute();
            if (user.empty()) {
                respond_json->set("code", static_cast<int>(state_code::LOGIN_USERNAME_NOT_EXIST));
                respond_json->set("msg", "用户名不存在");
                return HttpParamJSON(HTTPStatus::HTTP_NOT_FOUND, respond_json);
            }
            stored_salt = user[0]->salt();
            stored_password = user[0]->password();
            user_id = user[0]->id();
        }
        Poco::Crypto::DigestEngine engine("SHA256");
        engine.update(password + stored_salt);
        string provided_hashed_password = Poco::DigestEngine::digestToHex(engine.digest());
        if (provided_hashed_password == stored_password) {
            Poco::JSON::Object::Ptr data_json = new Poco::JSON::Object;
            data_json->set("id", user_id);
            data_json->set("username", username);
            respond_json->set("data", data_json);
            respond_json->set("code", static_cast<int>(state_code::SUCCESS));
            respond_json->set("msg", "登录成功");

            ///We need to generate a token for the user
            return HttpParamJSON(HTTPStatus::HTTP_OK, respond_json);
        } else {
            respond_json->set("code", static_cast<int>(state_code::LOGIN_PASSWORD_ERROR));
            respond_json->set("msg", "密码错误");
            return HttpParamJSON(HTTPStatus::HTTP_UNAUTHORIZED, respond_json);
        }
    }
    catch (Poco::Exception &e) {
        // If any other exception occurs, log it and return a server error.
        poco_error(Application::instance().logger(), e.displayText());
        respond_json->set("code", static_cast<int>(state_code::USER_SERVER_ERROR));
        respond_json->set("msg", "服务器错误");
        return HttpParamJSON(HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, respond_json);
    }
}
bool UserManager::insertMsg(int user_id, int room_id, std::string message) {
    try {
        auto session = DataFacade::getSession();
        Poco::ActiveRecord::Context::Ptr p_context = new Poco::ActiveRecord::Context(session);
        ChatRoomDB::User::Ptr p_user = ChatRoomDB::User::find(p_context, user_id);
        if (p_user == nullptr) {
            return false;
        }
        ChatRoomDB::Room::Ptr p_room = ChatRoomDB::Room::find(p_context, room_id);
        if (p_room == nullptr) {
            return false;
        }
        ChatRoomDB::Message::Ptr p_message = new ChatRoomDB::Message;
        p_message->sender_id(p_user);
        p_message->room_id(p_room);
        p_message->content(message);
        p_message->timestamp(Poco::Timestamp().epochTime());
        p_message->create(p_context);
        return true;
    }
    catch (Poco::Exception &e) {
        poco_error(Application::instance().logger(), e.displayText());
        return false;
    }
}

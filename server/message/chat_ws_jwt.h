//
// Created by lambert on 23-6-8.
//

#pragma once
#include <optional>
#include "state_code.h"

inline std::optional<std::tuple<int, std::string>> VerifyJwt(Poco::JSON::Object::Ptr &recvObject,
                                                             Poco::JSON::Object::Ptr &sendObject) {
    try {
        if (!recvObject->has("type") || recvObject->getValue<std::string>("type") != "Authorization/jwt") {
            throw Poco::Exception("No Authorization");
        }

        auto content = recvObject->getValue<std::string>("content");

        if (content.substr(0, 7) == "Bearer ") {
            std::string jwt = content.substr(7);

            Poco::JWT::Signer signer{g_JWT_secret};

            auto token = signer.verify(jwt);
            int id = token.payload().get("user_id");
            string user_name = token.payload().get("username");

            // Create success message
            sendObject->set("type", "Authorization/jwt");
            sendObject->set("timestamp", static_cast<int>(Poco::Timestamp().epochTime()));
            {
                Poco::JSON::Object::Ptr sender{new Poco::JSON::Object};

                sender->set("id", 100);
                sender->set("name", "server");
                sendObject->set("sender", sender);
            }

            {
                Poco::JSON::Object::Ptr content{new Poco::JSON::Object};

                content->set("code", 0);
                content->set("msg", "JWT verified!");
                sendObject->set("content", content);
            }

            return std::make_tuple(id, user_name);

        } else {
            throw Poco::Exception("Invalid Authorization content");
        }
    } catch (Poco::Exception &e) {
        // Create failure message
        poco_information_f1(Application::instance().logger(), "JWT verification failed: %s", e.what());
        sendObject->set("type", "Authorization/jwt");
        sendObject->set("timestamp", static_cast<int>(Poco::Timestamp().epochTime()));
        {
            Poco::JSON::Object::Ptr sender{new Poco::JSON::Object};

            sender->set("id", 100);
            sender->set("name", "server");
            sendObject->set("sender", sender);
        }
        {
            Poco::JSON::Object::Ptr content{new Poco::JSON::Object};

            content->set("code", static_cast<int>(state_code::VERIFY_TOKEN_FAILED));
            content->set("msg", "JWT verification failed!");
            sendObject->set("content", content);
        }

        return std::nullopt;
    }
}
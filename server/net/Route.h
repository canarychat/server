//
// Created by lambert on 23-5-28.
//

#pragma once

#include <string>
#include <functional>
#include <regex>

#include "poco_headers.h"
#include "DataManager.h"
#include "chat_jwt.h"
#include "state_code.h"
#include "DataFacade.h"

struct RouteKey {
    string requestType;
    std::regex pattern;
};
using Route = std::pair<RouteKey, std::function<void(HTTPServerRequest &, HTTPServerResponse &)>>;
inline DataManager &DM_instance() {
    return Poco::Util::Application::instance().getSubsystem<DataManager>();
}
inline std::vector<Route> routeTable{
    //CORS
    {
        {"OPTIONS", std::regex("/.*")},
        [](HTTPServerRequest &request, HTTPServerResponse &response) {
            // 设置CORS响应头
//          poco_debug_f1(Application::instance().logger(), "request.getURI() = %s", request.getURI());
            response.add("Access-Control-Allow-Origin", "*");  // 这里你也可以设置为你的前端域名，而不是'*'
            response.add("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE, PUT");
//            response.add("Access-Control-Allow-Headers", "Authorization, Content-Type");
//            response.set("Access-Control-Expose-Headers", "Authorization");


            response.set("Author", "Lambert");
            response.send();
        }
    },

    // Register
    {{"POST", std::regex("/register")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         poco_debug_f1(Application::instance().logger(), "request.getURI() = %s", request.getURI());
         response.add("Access-Control-Allow-Origin", "*");
         response.add("Access-Control-Allow-Headers", "Authorization");

         response.setContentType("application/json");

         std::ostream &ostr = response.send();

         Poco::JSON::Parser parser;
         auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();

         // 处理JSON数据...
         auto username = res->getValue<string>("username");
         auto password = res->getValue<string>("password");
         auto email = res->getValue<string>("email");

         if (username.empty() || password.empty()) {
             response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
             return;
         }

         Poco::JSON::Object::Ptr json = DataFacade::registerUser(username, password, email);
         json->stringify(ostr);
     }},


    // Login
    {{"POST", std::regex("/login")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle login request
         response.setContentType("application/json");
         response.add("Access-Control-Allow-Origin", "*");
         response.add("Access-Control-Allow-Methods", "POST");
//         response.add("Access-Control-Allow-Headers", "Authorization, Content-Type");
//         response.set("Access-Control-Expose-Headers", "Authorization");

         try {
             string jwt = request.get("Authorization", "");
             if (!jwt.empty())
                 jwt = jwt.substr(7);
             Poco::JWT::Signer signer(CONST_CONFIG::kTokenSecret);
             Poco::JWT::Token token = signer.verify(jwt);
             //We only process situation that token is valid
             if (token.getExpiration() > Poco::Timestamp()) {
                 Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                 json->set("code", 0);
                 json->set("msg", "JWT验证成功");
                 Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
                 data->set("username", token.payload().get("username"));
                 data->set("id", token.payload().get("id"));
                 data->set("email", token.payload().get("email"));
                 json->set("data", data);
                 json->stringify(response.send());
             }

         }
         catch (Poco::Exception &e) {

             Poco::JSON::Parser parser;
             auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();
             auto username = res->getValue<string>("username");
             auto password = res->getValue<string>("password");
             auto id = res->getValue<int>("id");

             poco_information_f1(Application::instance().logger(), "Exception: %s", e.displayText());
             Poco::JSON::Object::Ptr json = DataFacade::loginUser(username, id, password);
             if (json->getValue<int>("code") == 0) {
                 response.set("Authorization", "Bearer " + setJWT(id, username));
             }
             json->stringify(response.send());
         }

     }},
    // Get Chatrooms List
    {{"GET", std::regex("/chatrooms")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {

         int id;
         try {
             auto jwt = request.get("Authorization", "");
             Poco::JWT::Signer signer{CONST_CONFIG::kTokenSecret};
             Poco::JWT::Token token = signer.verify(jwt);
             id = token.payload().get("user_id");
         }
         catch (Poco::Exception &e) {
             Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
             json->set("code", static_cast<int>(state_code::VERIFY_TOKEN_FAILED));
             json->set("msg", "JWT验证失败");
             json->stringify(response.send());
         }

         Poco::JSON::Object::Ptr result = DataFacade::getRoomList(id);
            result->stringify(response.send());
     }},
    // Create Chatroom
    {{"POST", std::regex("/chatroom")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle create chatroom request
     }},
    // Get Chatroom Info
    {{"GET", std::regex("/chatrooms/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get chatroom info request
     }},
    // Get Chatroom Member Info
    {{"GET", std::regex("/chatrooms/.+/member")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get chatroom member info request
     }},
    // Update Chatroom Info
    {{"PUT", std::regex("/chatrooms/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle update chatroom info request
     }},
    // Delete Chatroom
    {{"DELETE", std::regex("/chatrooms/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle delete chatroom request
     }},
    // Add Chatroom Member
    {{"POST", std::regex("/chatrooms/.+/member")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle add chatroom member request
     }},
    // Delete Chatroom Member
    {{"DELETE", std::regex("/chatrooms/.+/member")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle delete chatroom member request
     }},
    // Get Users List
    {{"GET", std::regex("/users")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get users list request
     }},
    // Get User Info
    {{"GET", std::regex("/users/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get user info request
     }},
    // Update User Info
    {{"PUT", std::regex("/users/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle update user info request
     }},
    // Delete User
    {{"DELETE", std::regex("/users/.+")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle delete user request
     }},
    // Index Entrance
    {{"GET", std::regex("/")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle index entrance request
     }}
};

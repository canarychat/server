//
// Created by lambert on 23-5-28.
//

#pragma once

#include <string>
#include <functional>
#include <regex>
#include <utility>

#include "poco_headers.h"
#include "DataManager.h"
#include "chat_http_jwt.h"
#include "state_code.h"
#include "DataFacade.h"
#include "MsgFacade.h"

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
        {"OPTIONS", std::regex("^/.*")},
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
    {{"POST", std::regex("^/register$")},
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
    {{"POST", std::regex("^/login$")},
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
             Poco::JWT::Signer signer(g_JWT_secret);
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
             Poco::JSON::Object::Ptr json = DataFacade::loginUser(username, id, password);
             if (json->getValue<int>("code") == 0) {
                 ///login successfully
                 response.set("Authorization", "Bearer " + SetJwt(id, username));
             }
             json->stringify(response.send());
         }

     }},
    // Get ChatRooms List
    {{"GET", std::regex("^/chatrooms$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {

         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();
         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [id, string] = v.value();
             result = DataFacade::getRoomList(id);
             result->stringify(response.send());
         } else
             result->stringify(response.send());
     }},
    // Create Chatroom
    {{"POST", std::regex("^/chatroom$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();
         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [id, string] = v.value();
             Poco::JSON::Parser parser;
             auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();
             auto room_name = res->getValue<std::string>("name");
             auto room_description = res->getValue<std::string>("description");
             result = DataFacade::createRoom(id, room_name, room_description);
             result->stringify(response.send());
         } else
             result->stringify(response.send());

     }},
    // Get Chatroom Info
    {{"GET", std::regex("^/chatrooms/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get chatroom info request
         std::cout << "get chatroom info" << std::endl;
     }},
    // Get Chatroom Member Info
    {{"GET", std::regex("^/chatrooms/(\\d+)/member$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get chatroom member info request
         auto paths = std::vector<std::string>{};
         Poco::URI(request.getURI()).getPathSegments(paths);
         int room_id = std::stoi(paths[1]);

         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();
         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [id, string] = v.value();
             result = DataFacade::getRoomMemberList(room_id, id);
             result->stringify(response.send());
         } else
             result->stringify(response.send());

     }},
    // Update Chatroom Info
    {{"PUT", std::regex("^/chatrooms/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle update chatroom info request
     }},
    // Delete Chatroom
    {{"DELETE", std::regex("^/chatrooms/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [user_id, string] = v.value();

             auto paths = std::vector<std::string>{};
             Poco::URI(request.getURI()).getPathSegments(paths);
             int room_id = std::stoi(paths[1]);

             result = DataFacade::deleteRoom(room_id, user_id);
             result->stringify(response.send());
         } else
             result->stringify(response.send());
     }},
    // Add Chatroom Member
    {{"POST", std::regex("^/chatrooms/\\d+/member$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [id, string] = v.value();

             auto paths = std::vector<std::string>{};
             Poco::URI(request.getURI()).getPathSegments(paths);
             int room_id = std::stoi(paths[1]);

             Poco::JSON::Parser parser;
             auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();
             auto member_id = res->getValue<int>("id");

             if (member_id != id) {
                 response.setStatus(HTTPResponse::HTTP_FORBIDDEN);
                 response.send();
                 return;
             }
             result = DataFacade::joinRoom(room_id, member_id);
             result->stringify(response.send());
         } else
             result->stringify(response.send());
     }},

    // Delete Chatroom Member
    {{"DELETE", std::regex("^/chatrooms/(\\d+)/member$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

         auto v = VerifyJwt(request, result);
         if (v.has_value()) {
             auto [id, string] = v.value();

             auto paths = std::vector<std::string>{};
             Poco::URI(request.getURI()).getPathSegments(paths);
             int room_id = std::stoi(paths[1]);

             Poco::JSON::Parser parser;
             auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();
             auto member_id = res->getValue<int>("id");

             if (member_id != id) {
                 response.setStatus(HTTPResponse::HTTP_FORBIDDEN);
                 response.send();
                 return;
             }
             result = DataFacade::leaveRoom(room_id, member_id);
             result->stringify(response.send());
         } else
             result->stringify(response.send());
     }},

    // Get Users List
    {{"GET", std::regex("^/users$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get users list request
     }},
    // Get User Info
    {{"GET", std::regex("^/users/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get user info request
     }},
    // Update User Info
    {{"PUT", std::regex("^/users/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle update user info request
     }},
    // Delete User
    {{"DELETE", std::regex("^/users/\\d+$")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle delete user request
     }}
};




inline std::map<std::string, std::function<void(Poco::JSON::Object::Ptr, Poco::JSON::Object::Ptr)>> WS_HandlerMap = {
    {"text/plain", [](Poco::JSON::Object::Ptr recv_json, Poco::JSON::Object::Ptr send_json) {
        MsgFacade::sendMsg(std::move(recv_json), std::move(send_json));
    }},
};
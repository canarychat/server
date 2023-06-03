//
// Created by lambert on 23-5-28.
//

#pragma once

#include <string>
#include <functional>
#include <regex>

#include "poco_headers.h"
#include "DataManager.h"

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
            response.add("Access-Control-Allow-Headers", "Content-Type");
            response.send();
        }
    },

    // Register
    {{"POST", std::regex("/register")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         poco_debug_f1(Application::instance().logger(), "request.getURI() = %s", request.getURI());
         response.add("Access-Control-Allow-Origin", "*");
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

         auto json = DM_instance().registerUser(username, password, email);
         json->stringify(ostr);
     }},


    // Login
    {{"POST", std::regex("/login")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle login request
         response.add("Access-Control-Allow-Origin", "*");
         response.setContentType("application/json");
         std::ostream &ostr = response.send();

         Poco::JSON::Parser parser;
         auto res = parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();

         // 处理JSON数据...
         auto username = res->getValue<string>("username");
         auto password = res->getValue<string>("password");
         auto id = res->getValue<int>("id");

         if ((username.empty() && id == 0) || password.empty()) {
             response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
             return;
         }

         auto json = DM_instance().loginUser(username,  id, password);
         json->stringify(ostr);
     }},
    // Get Chatrooms List
    {{"GET", std::regex("/chatrooms")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
         // handle get chatrooms list request
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

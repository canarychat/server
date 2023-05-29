//
// Created by lambert on 23-5-28.
//

#pragma once

#include <string>
#include <functional>
#include <regex>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionCallback.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/Application.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

struct RouteKey {
  std::string requestType;
  std::regex pattern;
};
using Route = std::pair<RouteKey, std::function<void(HTTPServerRequest &, HTTPServerResponse &)>>;

inline std::vector<Route> routeTable{
    // Register
    {{"POST", std::regex("/register")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
       response.setChunkedTransferEncoding(true);
       response.setContentType("application/json");
       std::ostream &ostr = response.send();
       Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
       Poco::JSON::Object::Ptr dataObject = new Poco::JSON::Object;
       dataObject->set("id", 0);
       json->set("code", 0);
       json->set("msg", "ok");
       json->set("data", dataObject);

       json->stringify(ostr);
     }},
    // Login
    {{"POST", std::regex("/login")},
     [](HTTPServerRequest &request, HTTPServerResponse &response) {
       // handle login request
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

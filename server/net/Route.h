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
using Route = std::pair<RouteKey, std::function<void(Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &)>>;

inline std::vector<Route> routeTable{
    {{"GET", std::regex("/.*")},
     [](Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
      response.setChunkedTransferEncoding(true);
      response.setContentType("text/html");
      std::ostream &ostr = response.send();
      ostr << "<html><head><title>WebSocketServer</title></head>";
      ostr << "<body><h1>WebSocketServer</h1>";
      ostr << "<p><a href=\"/chat\">Chat</a></p>";
      ostr << "</body></html>";
    }}
};

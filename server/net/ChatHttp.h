//
// Created by lambert on 23-5-27.
//

#pragma once
#include "Route.h"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>

using Poco::Net::HTTPServer;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Subsystem;

class ChatHttpSubSystem : public Subsystem {
 public:
  const char *name() const override {
    return "ChatHttpSubSystem";
  }

  void initialize(Poco::Util::Application &app) override;

  void uninitialize() override;
 private:
  Poco::Net::ServerSocket *p_server_socket_;
  Poco::Net::HTTPRequestHandlerFactory *p_factory_;
  Poco::Net::HTTPServerParams *p_params_;
  Poco::Net::HTTPServer *p_server_;
};
class ChatHttpServer : HTTPServer {

};

class ChatHTTPRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override ;
};

class ChatWSRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override ;
};

class ChatHTTPRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  ChatHTTPRequestHandlerFactory() = default;
  Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {

      if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
          return new ChatWSRequestHandler;
      else
          return new ChatHTTPRequestHandler;
  }
};

 class RequestNotification: public Poco::Notification{
  public:
    RequestNotification(HTTPServerRequest& request, HTTPServerResponse& response):request_(request),response_(response){}
    HTTPServerRequest& request_;
    HTTPServerResponse& response_;
 };
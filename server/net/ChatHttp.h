//
// Created by lambert on 23-5-27.
//

#pragma once

#include "poco_headers.h"
#include "MessageManager.h"

class ChatHttpSubSystem : public Poco::Util::Subsystem {
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

// class RequestNotification: public Poco::Notification{
//  public:
//    RequestNotification(HTTPServerRequest& request, HTTPServerResponse& response):request_(request),response_(response){}
//    HTTPServerRequest& request_;
//    HTTPServerResponse& response_;
// };
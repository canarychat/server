//
// Created by lambert on 23-5-27.
//

#include "ChatHttp.h"

void ChatHttpSubSystem::initialize(Application &app) {// 子系统的初始化逻辑
  app.logger().information("ChatHttpSubSystem Init");
  auto port = app.config().getInt("MainServer.port", 8080);
  p_server_socket_ = new Poco::Net::ServerSocket(port);
  p_factory_ = new ChatHTTPRequestHandlerFactory();
  p_params_ = new Poco::Net::HTTPServerParams();
  p_server_ = new Poco::Net::HTTPServer(p_factory_, *p_server_socket_, p_params_);
  p_server_->start();
}
void ChatHttpSubSystem::uninitialize() {
    // 子系统的清理逻辑
    Application::instance().logger().information("ChatHttpSubSystem unInit");
    if (p_server_){
      p_server_->stop();
      delete p_server_;
    }
    delete p_factory_;
    delete p_server_socket_;
}
void ChatHTTPRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
  Application& app = Application::instance();
  app.logger().information("Request from " + request.clientAddress().toString());
}

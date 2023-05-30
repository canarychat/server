//
// Created by lambert on 23-5-27.
//

#include "ChatHttp.h"
#include <regex>

void ChatHttpSubSystem::initialize(Application &app) {// 子系统的初始化逻辑
  app.logger().information("ChatHttpSubSystem Init");
  auto port = app.config().getInt("MainServer.port", 12300);
  p_server_socket_ = new Poco::Net::ServerSocket(port);
  p_factory_ = new ChatHTTPRequestHandlerFactory();
  p_params_ = new Poco::Net::HTTPServerParams();

  //set params
  auto thread_number = app.config().getInt("MainServer.threadNumber", 16);
  p_params_->setMaxThreads(16);
  p_params_->setMaxQueued(64);
  p_params_->setThreadIdleTime(100);

  p_server_ = new Poco::Net::HTTPServer(p_factory_, *p_server_socket_, p_params_);
  p_server_->start();
  poco_information_f1(app.logger(), "ChatHttpSubSystem start at %d", port);
}
void ChatHttpSubSystem::uninitialize() {
  // 子系统的清理逻辑
  Application::instance().logger().information("ChatHttpSubSystem unInit");
  if (p_server_) {
    p_server_->stop();
    delete p_server_;
  }
  delete p_factory_;
  delete p_server_socket_;
}
void ChatHTTPRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
  Application &app = Application::instance();
  auto request_method = request.getMethod();
  auto request_uri = request.getURI();
  poco_information_f2(app.logger(), "request method: %s request uri: %s", request_method, request_uri);
  for (const auto &route : routeTable) {
    std::regex route_regex(route.first.pattern);
    std::smatch match_result;
    if (std::regex_match(request_uri, match_result, route_regex)) {
      route.second(request, response);
      return;
    }
  }
  //Not Found
  std::ostream &ostr = response.send();
  Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
  json->set("code", 404);
  json->set("msg", "api not found");
  json->stringify(ostr);
  poco_warning_f1(app.logger(), "api not found: %s", request_uri);
  response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

}

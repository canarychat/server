//
// Created by lambert on 23-5-27.
//

#include "ChatHttp.h"
#include "chat_ws_jwt.h"
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
    auto request_host = request.getHost();
    poco_information_f3(app.logger(),
                        "request method: %s, uri: %s, host: %s",
                        request_method,
                        request_uri,
                        request_host);

    response.set("server", "Lambert's ChatServer/api/v1");

    for (const auto &route : routeTable) {
        std::regex route_regex(route.first.pattern);
        if (route.first.requestType == request_method && std::regex_match(request_uri, route_regex)) {
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
void ChatWSRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    auto request_uri = request.getURI();
    auto request_host = request.getHost();
    poco_information_f2(app.logger(),
                        "request method: WS, uri: %s, host: %s",
                        request_uri,
                        request_host);

    response.set("server", "Lambert's ChatServer/api/v1");

    try {
        Poco::Net::WebSocket ws(request, response);
        char buffer[8192];
        int flags, recv;

        //Auth
        Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
        recv = ws.receiveFrame(buffer, sizeof(buffer), flags);

        Poco::JSON::Parser parser;
        auto recv_json = parser.parse(buffer).extract<Poco::JSON::Object::Ptr>();
        auto v = VerifyJwt(recv_json,result);
        if (!v.has_value()) {
            poco_information_f1(app.logger(), "auth failed: %s", result->get("msg").toString());
            std::ostringstream ostr;
            result->stringify(ostr);
            ws.sendFrame(ostr.str().c_str(), ostr.str().length(), flags);
            return;
        }

        do {
            recv = ws.receiveFrame(buffer, sizeof(buffer), flags);

            ws.sendFrame(buffer, recv, flags);
        }
        while (recv > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
    } catch (Poco::Net::WebSocketException &exc) {
        // Handle WebSocket exception
        poco_information(Application::instance().logger(), exc.displayText());
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
                // fallthrough
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
        }
    }
}

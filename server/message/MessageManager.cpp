//
// Created by lambert on 23-6-8.
//

#include "MessageManager.h"
#include "chat_ws_jwt.h"
#include "MsgFacade.h"
#include "Route.h"

void MessageManager::initialize(Application &app) {
    poco_information(Application::instance().logger(), "MessageManager init");
}
void MessageManager::sendMsg(Poco::JSON::Object::Ptr recv_json, Poco::JSON::Object::Ptr send_json) {
    try {
        // TODO:验证权限
        //if (!VerifyUserPermission(recv_json))
        //    throw Poco::Exception("Permission denied");

        if (recv_json->has("content")) {
            auto content = recv_json->getObject("content");

            int room_id = content->get("room_id");
            auto message = content->get("message").convert<std::string>();

            // 构建待插入的消息对象
            Poco::JSON::Object::Ptr msg = new Poco::JSON::Object();
            msg->set("sender_id", recv_json->getObject("sender")->get("id"));
            msg->set("sender_name", recv_json->getObject("sender")->get("name"));
            msg->set("timestamp", Poco::Timestamp().epochTime());
            msg->set("type", "text/plain");
            msg->set("content", message);

            postMessage(room_id, msg);

            // 插入到Redis
            std::string key = "chatroom:" + std::to_string(room_id) + ":message";
            Poco::Redis::Client redis("localhost", 6379);
            std::ostringstream oss;
            msg->stringify(oss);
            Poco::Redis::Command clp = Poco::Redis::Command::lpush(key, oss.str());
            redis.execute<Poco::Int64>(clp);
            //TODO: 异步插入到数据库

        } else {
            throw Poco::Exception("No message content found");
        }
    } catch (Poco::Exception &e) {
        poco_warning(Application::instance().logger(), e.what());
        // 处理出错的情况
        send_json->set("type", "text/plain");
        send_json->set("timestamp", Poco::Timestamp().epochTime());
    }
}

void ChatWSRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    auto request_uri = request.getURI();
    auto request_host = request.getHost();
    poco_information_f2(app.logger(),
                        "request method: WS, uri: %s, host: %s",
                        request_uri,
                        request_host);
    //获取room_id


    response.set("server", "Lambert's ChatServer/api/v1");

    try {
        Poco::Net::WebSocket ws(request, response);
        char buffer[8192];
        int flags, recv_length;
        //Auth
        Poco::JSON::Object::Ptr send_json = new Poco::JSON::Object;
        ws.receiveFrame(buffer, sizeof(buffer), flags);

        Poco::JSON::Parser parser;
        auto recv_json = parser.parse(buffer).extract<Poco::JSON::Object::Ptr>();
        auto v = VerifyJwt(recv_json, send_json);
        if (!v.has_value()) {
            poco_information_f1(app.logger(), "auth failed: %s", send_json->get("msg").toString());
            std::ostringstream ostr;
            send_json->stringify(ostr);
            ws.sendFrame(ostr.str().c_str(), ostr.str().length(), flags);
            return;
        }
        //Auth success, continue listening

        auto paths = std::vector<std::string>{};
        Poco::URI(request.getURI()).getPathSegments(paths);
        int room_id = std::stoi(paths[1]);

        MessageBroadcaster listener(ws, room_id);
        ///Use this to listen msg from other users
        do {
            memset(buffer, 0, sizeof(buffer));
            recv_length = ws.receiveFrame(buffer, sizeof(buffer), flags);
            parser.reset();
            recv_json = parser.parse( buffer).extract<Poco::JSON::Object::Ptr>();
            //查询WS伪路由表，处理消息
            WS_HandlerMap[recv_json->get("type").toString()](recv_json, send_json);
            std::ostringstream ostr;
            send_json->stringify(ostr);
        }
        while (recv_length > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
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
void MessageBroadcaster::onMessage(const Poco::AutoPtr<WS_MesageNotification> &pNf) {
    try {
        // Get the message from the notification
        Poco::JSON::Object::Ptr msg = pNf->getPayload();

        // Convert JSON message to string
        std::ostringstream ostr;
        msg->stringify(ostr);

        // Send the message through WebSocket
        std::string messageStr = ostr.str();
        ws_.sendFrame(messageStr.c_str(), messageStr.size());

    } catch (Poco::Exception &exc) {
        // Handle exceptions that might occur during sending
        poco_warning_f1(Application::instance().logger(), "Failed to broadcast message: %s", exc.displayText());
    }
}


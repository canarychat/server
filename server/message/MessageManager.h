//
// Created by lambert on 23-6-8.
//

#pragma once


#include "poco_headers.h"

class WS_MesageNotification : public Poco::Notification {
    ///用来传递消息的Notification
  public:
    using Ptr = Poco::AutoPtr<WS_MesageNotification>;
    WS_MesageNotification(Poco::JSON::Object::Ptr json, int room_id) : payload(json), room_id(room_id) {}
    Poco::JSON::Object::Ptr getPayload() const {
        return payload;
    }
  private:
    Poco::JSON::Object::Ptr payload;
    int room_id;
};

class MessageManager : public Poco::Util::Subsystem {
    /// 负责管理消息以及Websocket的Subsystem,注册在MainServerApplication之下
  public:
    explicit MessageManager() = default;
    const char *name() const override {
        return "MessageManager";
    }

    void initialize(Poco::Util::Application &app) override;

    void uninitialize() override {
        // 子系统的清理逻辑
        Application::instance().logger().information("MessageManager unInit");
    }

    void sendMsg(Poco::JSON::Object::Ptr recv_json, Poco::JSON::Object::Ptr send_json);

    void postMessage(int room_id, Poco::JSON::Object::Ptr json)
    ///把需要广播的消息发送给对应的房间
    {
        //这里没有创建center, 因为center在用户认证后创建
        if (room_centers_.find(room_id) != room_centers_.end()) {
            room_centers_[room_id]->postNotification(new WS_MesageNotification(json, room_id));
        }
    }

    Poco::NotificationCenter &getCenter(const int &room_id)
    ///获取一个房间的NotificationCenter
    {
        if (room_centers_.find(room_id) == room_centers_.end()) {
            room_centers_[room_id] = new Poco::NotificationCenter;
        }
        return *room_centers_[room_id];
    }

  private:
    std::map<int, Poco::NotificationCenter *> room_centers_;
};

class ChatWSRequestHandler : public Poco::Net::HTTPRequestHandler {
    ///处理Websocket的RequestHandler
  public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

class MessageBroadcaster {
  public:
    MessageBroadcaster(Poco::Net::WebSocket &ws,int room_id)
        : ws_(ws), room_id_(room_id), observer_(*this, &MessageBroadcaster::onMessage) {
        Application::instance().getSubsystem<MessageManager>().getCenter(room_id).addObserver(observer_);
    }
    ~MessageBroadcaster() {
        Application::instance().getSubsystem<MessageManager>().getCenter(room_id_).removeObserver(observer_);
    }

    void onMessage(const Poco::AutoPtr<WS_MesageNotification> &pNf);

  private:
    Poco::Net::WebSocket &ws_;
    int room_id_;
    Poco::NObserver<MessageBroadcaster, WS_MesageNotification> observer_;
};


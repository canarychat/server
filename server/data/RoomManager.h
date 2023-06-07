//
// Created by lambert on 23-6-4.
//

#pragma once

#include <utility>

#include "poco_headers.h"

class RoomManager : public Poco::Util::Subsystem {
  public:
    RoomManager() = default;
    const char *name() const override {
        return "RoomManager";
    }

    void initialize(Poco::Util::Application &app) override;

    void uninitialize() override {
        // 子系统的清理逻辑
        Application::instance().logger().information("UserManager unInit");
    }

    //功能函数
    Poco::JSON::Object::Ptr getRoomList(int user_id);

    Poco::JSON::Object::Ptr createRoom(int owner_id, std::string room_name, std::string description="");

    Poco::JSON::Object::Ptr deleteRoom(int room_id);

  private:

};

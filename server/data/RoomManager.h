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
    HttpParamJSON getRoomList(int user_id);

    HttpParamJSON createRoom(int owner_id, std::string room_name, std::string description="");

    HttpParamJSON deleteRoom(int room_id,int user_id);

    HttpParamJSON getRoomInfo(int room_id);

    HttpParamJSON getRoomMemberList(int room_id,int user_id);

    HttpParamJSON joinRoom(int room_id,int user_id);

    HttpParamJSON leaveRoom(int room_id,int user_id);

  private:

    HttpParam<> check_exit(Poco::JSON::Object::Ptr result,Poco::ActiveRecord::Context::Ptr, int room_id, int user_id);
};

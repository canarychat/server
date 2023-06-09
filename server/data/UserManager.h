//
// Created by lambert on 23-6-3.
//

#pragma once

#include "poco_headers.h"

class UserManager : public Poco::Util::Subsystem {
  public:
    explicit UserManager() {}
    const char *name() const override {
        return "UserManager";
    }

    void initialize(Poco::Util::Application &app) override;

    void uninitialize() override {
        // 子系统的清理逻辑
        Application::instance().logger().information("UserManager unInit");
    }

    //功能函数
    HttpParamJSON registerUser(const std::string &username,
                                                    const std::string &password,
                                                    const std::string &email = "");

    HttpParamJSON loginUser(const std::string &username, const int &user_id, const std::string &password);

    bool insertMsg(int user_id, int room_id, std::string message);

  private:
};

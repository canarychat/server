//
// Created by lambert on 23-6-8.
//

#pragma once

#include "poco_headers.h"

class MessageManager : public Poco::Util::Subsystem {
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

  private:
};
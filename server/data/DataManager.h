//
// Created by lambert on 23-5-27.
//

#pragma once

#include <memory>

#include "poco_headers.h"
#include "state_code.h"

using Poco::Util::Subsystem;
using Poco::Util::Application;



class DataManager : public Subsystem {
  public:
    explicit DataManager(const string &config);
    const char *name() const override {
        return "DataManager";
    }

    void initialize(Poco::Util::Application &app) override;

    void uninitialize() override {
        // 子系统的清理逻辑
        Application::instance().logger().information("DataManager unInit");
    }

    Poco::Data::Session getSession() {
        return session_pool_.get();
    }

  private:
    Poco::Data::SessionPool session_pool_;
    friend struct DataFacade;
};

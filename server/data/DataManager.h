//
// Created by lambert on 23-5-27.
//

#pragma once
#include "Poco/Util/Subsystem.h"
#include <Poco/Util/Application.h>

using Poco::Util::Subsystem;
using Poco::Util::Application;

class DataManager : public Subsystem {
 public:
  const char* name() const override
  {
    return "DataManager";
  }

  void initialize(Poco::Util::Application& app) override
  {
    // 子系统的初始化逻辑
    Application::instance().logger().information("DataManager Init");
  }

  void uninitialize() override
  {
    // 子系统的清理逻辑
    Application::instance().logger().information("DataManager unInit");
  }
};


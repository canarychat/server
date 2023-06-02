//
// Created by lambert on 23-5-27.
//

#pragma once
#include "poco_headers.h"
#include "state_code.h"

using Poco::Util::Subsystem;
using Poco::Util::Application;

class DataManager : public Subsystem {
 public:
  DataManager() =default;
  const char* name() const override
  {
    return "DataManager";
  }

  void initialize(Poco::Util::Application& app) override;

  void uninitialize() override
  {
    // 子系统的清理逻辑
    Application::instance().logger().information("DataManager unInit");
  }

  //功能函数
  Poco::JSON::Object::Ptr registerUser(const std::string& username, const std::string& password, const std::string& email="");
 private:
    // 数据库连接
    std::unique_ptr<Poco::Data::Session> p_session_;
    // 数据库上下文
    Poco::ActiveRecord::Context::Ptr p_context_= nullptr;
};

//class RequestOberver{
// public:
//    RequestOberver() = default;
//  void onNotification(RequestNotification::Ptr pNf)
//  {
//  }
//};
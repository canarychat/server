//
// Created by lambert on 23-6-3.
//

#pragma once

#include "poco_headers.h"

class UserManager: public Poco::Util::Subsystem{
    public:
    UserManager( Poco::ActiveRecord::Context::Ptr p_context):p_context_(p_context){   }
    const char* name() const override
    {
        return "UserManager";
    }

    void initialize(Poco::Util::Application& app) override;

    void uninitialize() override
    {
        // 子系统的清理逻辑
        Application::instance().logger().information("UserManager unInit");
    }


    //功能函数
    Poco::JSON::Object::Ptr registerUser(const std::string& username, const std::string& password, const std::string& email="");

    Poco::JSON::Object::Ptr loginUser(const std::string& username, const int &user_id, const std::string& password);

  private:
    private:
        // 数据库上下文
        Poco::ActiveRecord::Context::Ptr p_context_= nullptr;
};

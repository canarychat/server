//
// Created by lambert on 23-6-8.
//

#pragma once
#include <utility>

#include "MessageManager.h"

struct MsgFacade{
    static void sendMsg(Poco::JSON::Object::Ptr recv_json, Poco::JSON::Object::Ptr send_json){
        Application::instance().getSubsystem<MessageManager>().sendMsg(std::move(recv_json), std::move(send_json));
    }
};
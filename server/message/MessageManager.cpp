//
// Created by lambert on 23-6-8.
//

#include "MessageManager.h"
#include "chat_ws_jwt.h"

void MessageManager::initialize(Application &app) {
    poco_information(Application::instance().logger(), "MessageManager init");
}

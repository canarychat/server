//
// Created by lambert on 23-5-27.
//

#include "DataManager.h"

#include "poco_headers.h"
#include "ChatRoomDB/User.h"
#include "UserManager.h"
#include "RoomManager.h"


DataManager::DataManager(const string& config):
    session_pool_("MySql",config, 1, 16, 180){


    Application::instance().addSubsystem(new UserManager);
    Application::instance().addSubsystem(new RoomManager);
}

void DataManager::initialize(Application &app) {
    poco_information(app.logger(), "DataManager init");

}






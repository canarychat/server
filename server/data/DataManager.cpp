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
    try {
        // ping
        int result = 0;
        auto session = session_pool_.get();
        session << "SELECT 1", Poco::Data::Keywords::into(result), Poco::Data::Keywords::now;
    } catch (const Poco::Exception& e) {
        poco_error(Application::instance().logger(), e.displayText());
    }
    Application::instance().addSubsystem(new UserManager);
    Application::instance().addSubsystem(new RoomManager);
}

void DataManager::initialize(Application &app) {
    poco_information(app.logger(), "DataManager init");
}






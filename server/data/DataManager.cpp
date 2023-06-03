//
// Created by lambert on 23-5-27.
//

#include "DataManager.h"

#include "poco_headers.h"
#include "ChatRoomDB/User.h"
#include "UserManager.h"


void DataManager::initialize(Application &app) {
    poco_information(app.logger(), "DataManager init");
    try {
        Poco::Data::MySQL::Connector::registerConnector();
        auto data_config = app.config().getString("Data.mysql.config");
        p_session_ = std::make_unique<Poco::Data::Session>("MySQL", data_config);
        p_context_ = new Poco::ActiveRecord::Context(*p_session_);
    }
    catch (Poco::Exception &e) {
        poco_error(app.logger(), e.displayText());
        exit(1);
    }
    app.addSubsystem(new UserManager(p_context_));
}





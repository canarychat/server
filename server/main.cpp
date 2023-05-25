//
// Created by lambert on 22-12-11.
//

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

class MainServer : public Poco::Util::ServerApplication {
 public:
  MainServer() : _helpRequested(false) {
  }

  ~MainServer() = default;

 protected:

  void initialize(Application &self) {
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
  }

  void uninitialize() {
    ServerApplication::uninitialize();
  }

  void defineOptions(OptionSet &options) {
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false));
    options.addOption(
        Option("port", "p", "set port number")
            .required(false)
            .repeatable(false)
            .argument("number")
            .binding("port")
        );
  }

  void handleOption(const std::string &name, const std::string &value) {
    ServerApplication::handleOption(name, value);
    if (name == "help")
      _helpRequested = true;

  }

  void displayHelp(){
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A web server holds a chatroom server");
    helpFormatter.format(std::cout);
  }

  int main(const std::vector<std::string> &args) {
    if (_helpRequested) {
      displayHelp();
    } else {
      waitForTerminationRequest();
    }
    return Application::EXIT_OK;
  }

 private:
  bool _helpRequested;
};

int main(int argc, char **argv) {
  MainServer server;
  return server.run(argc, argv);
}
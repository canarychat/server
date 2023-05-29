//
// Created by lambert on 22-12-11.
//

#include "DataManager.h"
#include "ChatHttp.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionCallback.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/Application.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"

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
    addSubsystem(new DataManager);
    addSubsystem(new ChatHttpSubSystem);
  }

  ~MainServer() = default;

 protected:

  void initialize(Application &self) {
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
    std::string pattern = config().getString("MainServer.logPattern","[%p] - %Y-%m-%d %H:%M:%S (%O:%u) %t");
    auto* pPF = new Poco::PatternFormatter(pattern);
    auto* pCC = new Poco::ConsoleChannel;
    auto* pFC = new Poco::FormattingChannel(pPF,pCC);

    this->logger().setChannel(pFC);
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

    poco_information(logger(),"MainServerStarted");
    if (_helpRequested) {
      displayHelp();
    } else {
      unsigned short port = (unsigned short) config().getInt("MainServer.port", 12300);
      std::string format(config().getString("MainServer.format", DateTimeFormat::SORTABLE_FORMAT));
      ThreadPool::defaultPool();
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
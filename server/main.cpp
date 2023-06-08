//
// Created by lambert on 22-12-11.
//

#include "DataManager.h"
#include "ChatHttp.h"

#include "support/poco_headers.h"
#include "chat_http_jwt.h"

class MainServer : public Poco::Util::ServerApplication {
  public:
    MainServer() : _helpRequested(false) {

        //使用环境变量配置数据库
        string connectionString;
        if (Poco::Environment::has("DATABASE_CONFIG")) {
            connectionString = Poco::Environment::get("DATABASE_CONFIG");
            Poco::Data::MySQL::Connector::registerConnector();
        } else {
            poco_error(logger(), "DATABASE_CONFIG not found");
            exit(1);
        }
        poco_information(logger(), "MySQL connected successfully");
        if (Poco::Environment::has("JWT_SECRET")) {
            poco_information(logger(), "JWT_SECRET found in environment");
            g_JWT_secret = Poco::Environment::get("JWT_SECRET");
        } else {
            RandomGenerateJwtSecret();
            poco_information(logger(), "JWT_SECRET random generated");
        }

        try {
            addSubsystem(new DataManager(connectionString));
            addSubsystem(new ChatHttpSubSystem);
            addSubsystem(new MessageManager);
        }
        catch (Poco::Exception &e) {
            poco_error(logger(), e.displayText());
            exit(1);
        }
    }

    ~MainServer() = default;

  protected:

    void initialize(Application &self) {
        loadConfiguration(); // load default configuration files, if present
        ServerApplication::initialize(self);
        std::string pattern = config().getString("MainServer.logPattern", "[%p] -%J- %Y-%m-%d %H:%M:%S (%O:%u) %t");
        auto *pPF = new Poco::PatternFormatter(pattern);
        auto *pCC = new Poco::ConsoleChannel;
        auto *pFC = new Poco::FormattingChannel(pPF, pCC);

        this->logger().setChannel(pFC);
        this->logger().setLevel(config().getString("MainServer.logLevel", "information"));
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

    void displayHelp() {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A web server holds a chatroom server");
        helpFormatter.format(std::cout);
    }

    int main(const std::vector<std::string> &args) {

        poco_information(logger(), "MainServerStarted");
        if (_helpRequested) {
            displayHelp();
        } else {
            ThreadPool::defaultPool().addCapacity(config().getInt("MainServer.threadPoolCapacity", 16));
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
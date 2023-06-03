//
// Created by lambert on 23-5-31.
//

#pragma once

#include "const.h"

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
#include "Poco/JSON/Parser.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/ActiveRecord/Context.h"
#include "Poco/ActiveRecord/Query.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Notification.h"
#include "Poco/Exception.h"
#include "Poco/Crypto/DigestEngine.h"
#include <Poco/RandomStream.h>
#include <Poco/HexBinaryEncoder.h>
#include <Poco/HexBinaryDecoder.h>
#include <Poco/Base64Decoder.h>
#include <Poco/Base64Encoder.h>
#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JWT/Serializer.h>


using namespace Poco::Net;
using namespace Poco::ActiveRecord;

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
using Poco::ActiveRecord::Context;
using Poco::ActiveRecord::Query;

using std::string;
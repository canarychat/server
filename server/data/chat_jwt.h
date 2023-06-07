//
// Created by lambert on 23-6-3.
//

#pragma once

#include <optional>
#include "poco_headers.h"

inline std::string setJWT ( const int& user_id,const std::string& username){
    Poco::JWT::Signer signer{CONST_CONFIG::kTokenSecret};
    Poco::JWT::Token token;
    token.setIssuer("charServerLambert");
    token.payload().set("username", username);
    token.payload().set("user_id", user_id);
    token.setIssuedAt(Poco::Timestamp());
    token.setNotBefore(Poco::Timestamp());
    token.setExpiration(Poco::Timestamp()+Poco::Timespan(0,1,0,0,0));
    string jwt= signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    return jwt;
}

inline std::optional<std::tuple<int,string>> verifyJwt(Poco::Net::HTTPServerRequest &request, Poco::JSON::Object::Ptr &responseObject) {
    try {
        if (request.has("Authorization")) {
            auto jwt = request.get("Authorization", "").substr(7);
            Poco::JWT::Signer signer{CONST_CONFIG::kTokenSecret};
            Poco::JWT::Token token = signer.verify(jwt);
            int id = token.payload().get("user_id");
            Poco::Dynamic::Var user_name = token.payload().get("username");
            return std::make_tuple(id, user_name.toString());
        } else {
            throw Poco::Exception("No Authorization");
        }
    } catch (Poco::Exception &e) {
        responseObject = new Poco::JSON::Object();
        responseObject->set("code", static_cast<int>(state_code::VERIFY_TOKEN_FAILED));
        responseObject->set("msg", "JWT验证失败");
        return std::nullopt;
    }
}

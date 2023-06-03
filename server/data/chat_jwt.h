//
// Created by lambert on 23-6-3.
//

#pragma once

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
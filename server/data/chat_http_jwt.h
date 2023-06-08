//
// Created by lambert on 23-6-3.
//

#pragma once

#include <optional>

inline void RandomGenerateJwtSecret(){
    std::string salt;
    Poco::RandomInputStream rng;
    for (size_t i = 0; i < 24; ++i) {
        char c;
        rng >> c;
        salt += c;
    }
    std::stringstream ss;
    Poco::Base64Encoder base_64_encoder(ss);
    base_64_encoder << salt;
    base_64_encoder.close();  // 关闭编码器以刷新任何缓冲的数据
    g_JWT_secret = ss.str();
}

inline std::string SetJwt (const int& user_id, const std::string& username){
    Poco::JWT::Signer signer{g_JWT_secret};
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

 inline std::optional<std::tuple<int,string>> VerifyJwt(Poco::Net::HTTPServerRequest &request, Poco::JSON::Object::Ptr &response_object) {
    /// If verify successfully, verify shouldn't change the response json;
    try {
        if (auto jwt =request.get("Authorization", "");jwt.substr(0, 7) == "Bearer ") {
            jwt = jwt.substr(7);
            Poco::JWT::Signer signer{g_JWT_secret};
            Poco::JWT::Token token = signer.verify(jwt);
            int id = token.payload().get("user_id");
            Poco::Dynamic::Var user_name = token.payload().get("username");
            return std::make_tuple(id, user_name.toString());
        } else {
            throw Poco::Exception("No Authorization");
        }
    } catch (Poco::Exception &e) {
        response_object = new Poco::JSON::Object();
        response_object->set("code", static_cast<int>(state_code::VERIFY_TOKEN_FAILED));
        response_object->set("msg", "JWT验证失败");
        return std::nullopt;
    }
}
